#ifndef STRUCTS
#define STRUCTS

#include "vector.h"
#include <vector>

using namespace std;

struct Intersection {
    bool exists;
    double dist;
    Vector point;
    Vector normal;
    int index;
};

struct Camera {
    Vector center;
    double distance;
};

struct Image {
    int width;
    int height;
    vector<uint8_t> pixels;
};

#endif