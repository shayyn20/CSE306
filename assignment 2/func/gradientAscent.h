#include "optimalTransport.h"

std::vector<double> gradAscent(std::vector<Vector>& points, std::vector<double>& weights, double f, 
                                double step = 0.0001, double tol = 0.00002, int maxit = 1000) {
    std::vector<Polygon> cells = powerDiagram(points, weights);
    for (int i = 0; i < int(points.size()); i ++) {
        int it = 0;
        while (it < maxit) {
            double grad = g_grad(points[i], cells[i], f);
            if (abs(grad) <= tol) {
                break;
            }
            else {
                weights[i] += step * grad;
                it ++;
            }
        }
    }
    return weights;
}
