#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdint.h>
#include <stdlib.h>
#include "Histogram.hpp"
#include "main.hpp"
using namespace std;

// running in docker:
// docker run --rm -e DEV_UID=`id -u` -e DEV_GID=`id -u` -v ~/cmake-examples/:/media/local -it matrim/cmake-examples:3.10.3 /media/local/test.sh
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
    cout << "the entire file content is in memory" << endl;
    cout << "First two bytes: " << setfill('0') << setw(2) << hex << *memblock + 0 << " " << *memblock + 1 << endl << endl;

    // calculate the histograms
    Histogram histogram;
    get_histograms(memblock, &histogram);
    
    // histogram.printRedHistogram();
    // histogram.printGreenRedHistogram();
    // histogram.printGreenBlueHistogram();
    // histogram.printBlueHistogram();
    
    // histogram.get2procLimitRedHistogram(true);
    // histogram.get2procLimitGreenRedHistogram(true);
    // histogram.get2procLimitGreenBlueHistogram(true);
    // histogram.get2procLimitBlueHistogram(true);

    cout << "max=" << histogram.get2proclimitMaximum() << endl;
    
    free(memblock);
  }
  else cout << "Unable to open file" << endl;
  return 0;
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