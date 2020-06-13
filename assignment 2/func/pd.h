#include "sh_pow.h"

#define QUAD Polygon({Vector(0.001,0.001), Vector(0.999,0.001), Vector(0.999,0.999), Vector(0.001,0.999)});

std::vector<Polygon> powerDiagram(std::vector<Vector>& points, std::vector<double>& weights) {
    std::vector<Polygon> out;
    // #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < int(points.size()); i++) {
        Polygon subj = QUAD;
        Polygon cell = sutherlandHodgman_pow(subj, points, weights, i);
        out.push_back(cell);
    }
    return out;
}