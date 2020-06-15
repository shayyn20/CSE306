#include "powerDiagram.h"

#define QUAD Polygon({Vector(0.001,0.001), Vector(0.999,0.001), Vector(0.999,0.999), Vector(0.001,0.999)});

std::vector<Vector> lloydIteration_vor(std::vector<Vector>& points, int maxit = 5) {
    std::vector<Vector> newPoints = points;
    int it = 0;

    while (it < maxit) {
        it ++;
        std::vector<Polygon> cells = voronoiParalLinEnum(newPoints);
        #pragma omp parallel for schedule(dynamic, 1)
        for (int i = 0; i < int(points.size()); i++) {
            newPoints[i] = centroid(cells[i]);
        }
    }
    return newPoints;
}

std::vector<Vector> lloydIteration_pow(std::vector<Vector>& points, std::vector<double>& weights, int maxit = 5) {
    std::vector<Vector> newPoints = points;
    int it = 0;
    
    while (it < maxit) {
        it ++;
        std::vector<Polygon> cells = powerDiagram(newPoints, weights);
        #pragma omp parallel for schedule(dynamic, 1)
        for (int i = 0; i < int(points.size()); i++) {
            newPoints[i] = centroid(cells[i]);
        }
    }
    return newPoints;
}