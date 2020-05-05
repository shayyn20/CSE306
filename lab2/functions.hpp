#include <random>
#include <math.h>
#include <stdio.h>
#include "vector.hpp"

static std::default_random_engine engine(10); 
static std::uniform_real_distribution<double> uniform(0, 1);

using namespace std;

void boxMuller(double stdev, double &x, double &y) {
    double r1 = uniform(engine);
    double r2 = uniform(engine);
    x = sqrt(-2 * log(r1)) * cos(2 * M_PI * r2) * stdev;
    y = sqrt(-2 * log(r1)) * sin(2 * M_PI * r2) * stdev;
}

Vector random_cos(const Vector &N) {
    double r1 = uniform(engine);
    double r2 = uniform(engine);
    double x = cos(2 * M_PI * r1) * sqrt(1 - r2);
    double y = sin(2 * M_PI * r1) * sqrt(1 - r2);
    double z = sqrt(r2);

    double min_N = min(min(abs(N[0]), abs(N[1])), abs(N[2]));
    Vector t1;
    for (int i = 0; i < 3; i++) {
        if (abs(N[i]) == min_N) {
            if (i == 0) {
                t1 = Vector(0, -N[2], N[1]);
                break;
            }
            if (i == 1) {
                t1 = Vector(-N[2], 0, N[0]);
                break;
            }
            if (i == 2) {
                t1 = Vector(-N[1], N[0], 0);
                break;
            }
        }
    }
    t1 = normalize(t1);
    Vector t2 = cross(N, t1);
    return t1 * x + t2 * y + N * z;
}

