#ifndef GEOMETRY
#define GEOMETRY

#include <string>
#include "ray.h"
#include "structs.h"

class Geometry {
public: 
    Vector albedo;
    string type;
    virtual Intersection intersect(Ray* ray) = 0;
};

#endif