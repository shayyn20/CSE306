#ifndef RAY
#define RAY

#include "vector.h"

using namespace std;

class Ray {
public:
    Vector origin;
    Vector unit;

    Ray(Vector o, Vector u) {
        origin = o;
        unit = u;
    };
};

#endif