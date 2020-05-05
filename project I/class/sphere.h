#ifndef SPHERE
#define SPHERE

#include "geometry.h"

using namespace std;

class Sphere: public Geometry {
public: 
    Vector center;
    double radius;

    Sphere() {}
    Sphere(Vector c, double r, Vector clr, string t) {
        this->center = c;
        this->radius = r;
        this->albedo = clr;
        this->type = t;
    }

    Intersection intersect(Ray* ray) {
        Intersection i;
        Vector O = ray->origin;
        Vector U = ray->unit;
        Vector C = this->center;
        double R = this->radius;
        double delta = pow(dot(U, O-C),2) - pow((norm(O-C)),2) + pow(R,2);
        if (delta < 0) {
            i.exists = false;
        }
        else {
            double t_1 = dot(U, C-O) - sqrt(delta);
            double t_2 = dot(U, C-O) + sqrt(delta);
            double t;
            if (t_2 < 0) {
                i.exists = false;
            }
            else {
                i.exists = true;
                if (t_1 >= 0) {
                    t = t_1;
                }
                else {
                    t = t_2;
                }
                i.dist = t;
                i.point = O + (U * t);
                i.index = -1;
                i.normal = normalize(i.point - C);
            }
        }
        return i;
    }
    
};

#endif