#ifndef MAIN_H
#define MAIN_H
 
 struct histogramDataStructure {
    int red[255];
    int greenRed[255];
    int greenBlue[255];
    int blue[255];
};

int main(int argc, char *argv[]);
void get_histograms(uint8_t * frame, histogramDataStructure & histogramData);
 
#endif