#include "sutherlandHodgman_vor.h"

#define QUAD Polygon({Vector(0.001,0.001), Vector(0.999,0.001), Vector(0.999,0.999), Vector(0.001,0.999)});

std::vector<Polygon> voronoiParalLinEnum(std::vector<Vector>& points) {
    std::vector<Polygon> out;
// #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < int(points.size()); i++) {
        Polygon subj = QUAD;
        Polygon cell = sutherlandHodgman_vor(subj, points, i);
        out.push_back(cell);
    }
    return out;
}