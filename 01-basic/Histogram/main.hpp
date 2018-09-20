#ifndef MAIN_H
#define MAIN_H
 
int main(int argc, char *argv[]);
void get_histograms(uint8_t *data, Histogram *histogram);
float calc_offset(uint8_t * frame);
float calculateNewExposureTime(uint8_t *data);

#define MAX_EXPOSURE_TIME 33.247260
#define TARGET_PERCENTILE_VALUE 0.8

#endif