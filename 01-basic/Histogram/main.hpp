#ifndef MAIN_H
#define MAIN_H
 
int main(int argc, char *argv[]);
void get_histograms(uint8_t *data, Histogram *histogram);
float calc_offset(uint8_t * frame);
 
#endif