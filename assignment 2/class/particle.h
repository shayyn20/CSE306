#include "polygon.h"

class Particle {  
public:
	Vector position;
    Vector velocity;
    double mass;
    Particle() {};
    Particle(Vector p, Vector v, double m) {
        position = p;
        velocity = v;
        mass = m;
    };
};