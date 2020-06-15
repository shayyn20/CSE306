#include "sutherlandHodgman_pow.h"

#define QUAD Polygon({Vector(0.001,0.001), Vector(0.999,0.001), Vector(0.999,0.999), Vector(0.001,0.999)});

std::vector<Polygon> powerDiagram(const std::vector<Vector>& points, const std::vector<double>& weights) {
    std::vector<Polygon> out;
    // #pragma omp parallel // for schedule(dynamic, 1)
    for (int i = 0; i < int(points.size()); i++) {
        Polygon subj = QUAD;
        const Polygon cell = sutherlandHodgman_pow(subj, points, weights, i);
        out.push_back(cell);
    }
    return out;
}