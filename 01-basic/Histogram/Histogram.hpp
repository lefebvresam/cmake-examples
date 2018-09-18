#ifndef HISTOGRAM_H
#define HISTOGRAM_H

class Histogram {
        int redHistogram[255];
        int greenRedHistogram[255];
        int greenBlueHistogram[255];
        int blueHistogram[255];
    public:
        Histogram();
        ~Histogram();
        int* getRedHistogram();
        int* getGreenRedHistogram();
        int* getGreenBlueHistogram();
        int* getBlueHistogram();
};

#endif