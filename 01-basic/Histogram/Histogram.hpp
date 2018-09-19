#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdint.h>

class Histogram {
        int redHistogram[255];
        int greenRedHistogram[255];
        int greenBlueHistogram[255];
        int blueHistogram[255];
    public:
        Histogram();
        ~Histogram();
        void incrementRedHistogram(uint8_t position);
        void incrementGreenRedHistogram(uint8_t position);
        void incrementGreenBlueHistogram(uint8_t position);
        void incrementBlueHistogram(uint8_t position);
        void printRedHistogram();
        void printGreenRedHistogram();
        void printGreenBlueHistogram();
        void printBlueHistogram();
};

#endif