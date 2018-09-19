#include "Histogram.hpp"
#include <stdint.h>
#include <iostream>
using namespace std;

Histogram::Histogram() {
    for (int i = 0; i < 256; i++) {
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

void printHistogram(const char *title, int *histogram) {
    cout << title;
    for (int i = 0; i < 256; i++) 
        cout << dec << histogram[i] << ' ';
    cout << endl << endl;
}

void Histogram::printRedHistogram() {
    printHistogram("Red Histogram: ", redHistogram);
 }

void Histogram::printGreenRedHistogram() {
    printHistogram("Green Red Histogram: ", greenRedHistogram);
}

void Histogram::printGreenBlueHistogram() {
    printHistogram("Green Blue Histogram: ", greenBlueHistogram);
}

void Histogram::printBlueHistogram() {
    printHistogram("Blue Histogram: ", blueHistogram);
}

float get2procLimit(const char *title, int *histogram, bool output) {
    if (output) cout << title << endl;
    int sum = 0;
    for (int i = 0; i < 256; i++) {
        sum += histogram[i];
    }
    if (output) cout << "sum=" << sum << endl;
    int twoproc = sum * 0.02;
    if (output) cout << "twoproc=" << twoproc << endl;
    int acc = 0, limit = 0;
    for (int i = 256; i > 0; i--) {
        acc += histogram[i-1];
        if (acc > twoproc) {
            if (output) cout << "acc=" << acc << endl;
            limit = i-1;
            break;
        }
    }
    if (output) cout << "limit=" << limit << endl;
    float limitfactor = (float)limit/256;
    if (output) cout << "float=" << limitfactor << endl << endl;
    return limitfactor;
}

float Histogram::get2procLimitRedHistogram(bool output) {
    return get2procLimit("Red Histogram: ", redHistogram, output);
}

float Histogram::get2procLimitGreenRedHistogram(bool output) {
    return get2procLimit("Green Red Histogram: ", greenRedHistogram, output);
}

float Histogram::get2procLimitGreenBlueHistogram(bool output) {
    return get2procLimit("Green Blue Histogram: ", greenBlueHistogram, output);
}

float Histogram::get2procLimitBlueHistogram(bool output) {
    return get2procLimit("Blue Histogram: ", blueHistogram, output);
}

float Histogram::get2proclimitMaximum() {
    float max = 0, current = 0;
    current = get2procLimitRedHistogram(false);
    if (current > max) max = current;
    current = get2procLimitGreenRedHistogram(false);
    if (current > max) max = current;
    current = get2procLimitGreenBlueHistogram(false);
    if (current > max) max = current;
    current = get2procLimitBlueHistogram(false);
    if (current > max) max = current;
    return max;
}