#include "Histogram.hpp"

Histogram::Histogram() {
    // initialaize data
    for (int i = 0; i < 255; i++) {
        redHistogram[i] = 0;
        greenRedHistogram[i] = 0;
        greenBlueHistogram[i] = 0;
        blueHistogram[i] = 0;
    }
}

Histogram::~Histogram() {    
}

int* Histogram::getRedHistogram() {
    return redHistogram;
}

int* Histogram::getGreenRedHistogram() {
    return greenRedHistogram;
}

int* Histogram::getGreenBlueHistogram() {
    return greenBlueHistogram;
}

int* Histogram::getBlueHistogram() {
    return blueHistogram;
}