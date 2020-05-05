#ifndef LIGHT
#define LIGHT

#include "sphere.h"

class Light: public Sphere {
public:
    double intensity;

    Light() {}
    Light(Vector c, double r, double intensity = 200000) {
        this->center = c;
        this->radius = r;
        this->albedo = Vector(1, 1, 1);
        this->type = "light";
        this->intensity = intensity;
    }
    
};

#endif