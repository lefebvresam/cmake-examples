#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <stdint.h>

class Histogram {
        int redHistogram[256];
        int greenRedHistogram[256];
        int greenBlueHistogram[256];
        int blueHistogram[256];
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
        float get2procLimitRedHistogram(bool output);
        float get2procLimitGreenRedHistogram(bool output);
        float get2procLimitGreenBlueHistogram(bool output);
        float get2procLimitBlueHistogram(bool output);
        float get2proclimitMaximum();
};

#endif