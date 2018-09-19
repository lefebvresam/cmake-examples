#include "Histogram.hpp"
#include <stdint.h>
#include <iostream>
using namespace std;

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

void Histogram::incrementRedHistogram(uint8_t position) {
    redHistogram[position]++;
}

void Histogram::incrementGreenRedHistogram(uint8_t position) {
    greenRedHistogram[position]++;
}

void Histogram::incrementGreenBlueHistogram(uint8_t position) {
    greenBlueHistogram[position]++;
}

void Histogram::incrementBlueHistogram(uint8_t position) {
    blueHistogram[position]++;
}

void Histogram::printRedHistogram() {
    cout << "Red Histogram: ";
    for (int i = 0; i < 255; i++) {
        cout << dec << redHistogram[i] << ' ';
    }
    cout << endl << endl;
}

void Histogram::printGreenRedHistogram() {
    cout << "Green Red Histogram: ";
    for (int i = 0; i < 255; i++) {
        cout << dec << greenRedHistogram[i] << ' ';
    }
    cout << endl << endl;
}

void Histogram::printGreenBlueHistogram() {
    cout << "Green Blue Histogram: ";
    for (int i = 0; i < 255; i++) {
        cout << dec << greenBlueHistogram[i] << ' ';
    }
    cout << endl << endl;
}

void Histogram::printBlueHistogram() {
    cout << "Blue Histogram: ";
    for (int i = 0; i < 255; i++) {
        cout << dec << blueHistogram[i] << ' ';
    }
    cout << endl << endl;
}