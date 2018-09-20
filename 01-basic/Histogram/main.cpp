#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdint.h>
#include <stdlib.h>
#include "Histogram.hpp"
#include "main.hpp"
using namespace std;

// running in docker:
// docker run --rm -e DEV_UID=`id -u` -e DEV_GID=`id -u` -v ~/cmake-examples/:/media/local -it matrim/cmake-examples:3.10.3 /media/local/test(_short).sh
//                                                        |- mounting part                                                   |-script to run
// checking the test data:
// hexdump -n 20 testdata.raw

int main(int argc, char *argv[])
{
  streampos size;
  char * memblock;

  ifstream file ("../testdata.raw", ios::in|ios::binary|ios::ate); // ate means point to end of file
  if (file.is_open())
  {
    size = file.tellg();
    cout << "file size = " << size << "B" << endl;
    uint8_t* memblock;
    memblock = (uint8_t*)malloc(size);
    file.seekg (0, ios::beg); // point to begin of file
    file.read((char*)memblock, size);
    file.close();
    cout << "the entire file content is in memory" << endl << endl;
    //cout << "First two bytes: " << setfill('0') << setw(2) << hex << *memblock + 0 << " " << *memblock + 1 << endl << endl;

    Histogram histogram;
    get_histograms(memblock, &histogram);
    cout << "locked = " << (checkExposureLock(&histogram) ? "true" : "false") << endl;
    cout << "new exposuretime = " << calculateNewExposureTime(memblock, &histogram) << endl;
       
    free(memblock);
  }
  else cout << "Unable to open file" << endl;
  return 0;
}

// check wether the observed percentile is in the range [TARGET_PERCENTILE_VALUE - 0,02] - [TARGET_PERCENTILE_VALUE + 0,02]
// true: lock, false: no lock
#define TARGET_PERCENTILE_VALUE 0.8
#define TARGET_PERCENTILE_VALUE_POSDEV 0.02
#define TARGET_PERCENTILE_VALUE_NEGDEV 0.02
bool checkExposureLock(Histogram *histogram) {
    float currentpercentile = histogram->get2proclimitMaximum();
    float maxpercentile = TARGET_PERCENTILE_VALUE + TARGET_PERCENTILE_VALUE_POSDEV;
    float targetpercentile = TARGET_PERCENTILE_VALUE;
    float minpercentile = TARGET_PERCENTILE_VALUE - TARGET_PERCENTILE_VALUE_NEGDEV;
    cout << "maxpercentile = " << maxpercentile << endl;
    cout << "targetpercentile = " << targetpercentile << endl;    
    cout << "minpercentile = " << minpercentile << endl;
    if (currentpercentile > maxpercentile || currentpercentile < minpercentile)
        return false;
    else 
        return true;
}


#define MAX_EXPOSURE_TIME 33.247260
float calculateNewExposureTime(uint8_t *data, Histogram *histogram) {
    
    // histogram->printRedHistogram();
    // histogram->printGreenRedHistogram();
    // histogram->printGreenBlueHistogram();
    // histogram->printBlueHistogram();
    
    // histogram->get2procLimitRedHistogram(true);
    // histogram->get2procLimitGreenRedHistogram(true);
    // histogram->get2procLimitGreenBlueHistogram(true);
    // histogram->get2procLimitBlueHistogram(true);

    float observed = histogram->get2proclimitMaximum();
    float offset = calc_offset(data);
    float target = TARGET_PERCENTILE_VALUE;
    float maxexptime = MAX_EXPOSURE_TIME;

    cout << "observed percentile value = " << observed << endl;
    cout << "offset black level = " << offset << endl;
    cout << "target percentile value = " << target << endl;
    cout << "max exposure time = " << maxexptime << "ms" << endl;
    
    float exposuretime = -1;
    if (offset >= target) {
        cout << "Abnormally high black level, exposure time will not be updated" << endl;
    } else  {
        if (offset >= observed) {
            cout << "Observed percentile value is below the black level, exposure time will be set to maximum" << endl;
            exposuretime = maxexptime;
        } else {
            float gain = (target-offset)/(observed-offset);
            exposuretime = min(maxexptime*gain, maxexptime);
            cout << "exposure time gain = " << gain << endl;
        }
    }
    return exposuretime;
}

void get_histograms(uint8_t *data, Histogram *histogram) {
    // Frame has 2218 lines, first 16 are optical black, next there are 6 not to be used lines.
    // The 'active' area, height 2160, is centered in the remaining area, so with a border of 18 leading and trailing lines.
    // The width is 3864 of which the active area is 3840, so 12 leading and trailing columns.
    int width = 3864;

    // Bayer-encoded frame has pixels of size 2 x 2, so we will process the frame this way
    // Even lines are encoded as: 0xR1[11:4]  0xG1[11:4]  0xR1[3:0]G1[3:0] + 4 bytes dummy
    // Odd  lines are encoded as: 0xG2[11:4]  0xB1[11:4]  0xG2[3:0]B1[3:0] + 4 bytes dummy
    int bytesPerSubPixelPair = 3;
    int lineLength = width * bytesPerSubPixelPair / 2 + 4; // 4 dummy bytes at the end of each line

    // Obtain the coordinates (expressed in 2x2 pixels) of the active area
    int rowStart = (16 + 6 + 18       ) / 2; // Skip optical black, ignored and effective margin lines
    int rowEnd   = (16 + 6 + 18 + 2160) / 2; // Skip optical black, ignored and effective margin lines
    int colStart = (12                ) / 2; // Skip                            effective margin columns
    int colEnd   = (12          + 3840) / 2; // Skip                            effective margin columns

    // We will ignore the 4 LSB's of each sub-pixel, 8-bit accuracy is adequate for the histograms
    // Pre-calculate some often used values
    int stride = 16;
    int colStep          = bytesPerSubPixelPair * stride;
    int colStartInBytes  = bytesPerSubPixelPair * colStart;
    int doubleLineLength = lineLength * 2;

    for (int row = rowStart; row < rowEnd; row+=stride)
    {
        // Calculate the offset to the first column of the row
        int offsetRowOne = doubleLineLength * row + colStartInBytes;
        int offsetRowTwo = offsetRowOne + lineLength;
        for (int col = colStart; col < colEnd; col+=stride)
        {
            // Increment the histogram bin corresponding to the current sub-pixel value
            // Note that mBayer is used to select the correct color for the current pixel coordinate
            histogram->incrementRedHistogram(0xFF & data[offsetRowOne    ]); // Top left - red
            histogram->incrementGreenRedHistogram(0xFF & data[offsetRowOne + 1]); // Top right - greenRed
            histogram->incrementGreenBlueHistogram(0xFF & data[offsetRowTwo    ]); // Bottom left - greenBlue
            histogram->incrementBlueHistogram(0xFF & data[offsetRowTwo + 1]); // Bottom right - blue

            offsetRowOne += colStep;
            offsetRowTwo += colStep;
        }
    }
}

#define OPTICAL_BLACK_WIDTH 3864
#define OPTICAL_BLACK_HEIGHT 16
float calc_offset(uint8_t * frame) {
    long sum = 0;
    uint8_t *in_tmp = frame;

    // Line format: (OPTICAL_BLACK_WIDTH times 3 bytes) + 4 bytes padding
    // Two 12-bit pixels per 3 bytes, format: P1[11:4] P2[11:4] P2[3:0]P1[3:0]
    for(int y = 0; y < OPTICAL_BLACK_HEIGHT; y++) { // line per line
        for(int x = 0; x < OPTICAL_BLACK_WIDTH / 2; x++) { // for each pixel (2x2)
            uint16_t tmp;
            // First Pixel: P1[11:4]P1[3:0]
            tmp = ((uint16_t)(*in_tmp & 0xFF) << 4) | (*(in_tmp + 2) & 0x0F);
            sum += tmp;
            // Second Pixel: P2[11:4]P2[3:0]
            tmp = ((uint16_t)(*(in_tmp + 1) & 0xFF) << 4) | ((*(in_tmp + 2) & 0xF0) >> 4);
            sum += tmp;
            in_tmp+=3; // move the pointer
        }
        // Skip 4 dummy bytes at end of each line
        in_tmp += 4;
    }
    //cout << "sum=" << dec << sum << endl;
    float avg = (float)sum/(OPTICAL_BLACK_WIDTH * OPTICAL_BLACK_HEIGHT);
    //cout << "avg=" << avg << endl;
    return avg/4095;
}