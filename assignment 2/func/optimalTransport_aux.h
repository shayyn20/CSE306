#include "lloydIter.h"

std::vector<double> uniform_lambda(const std::vector<Vector>& points) {
    std::vector<double> lambdas;
    int n = int(points.size());
    for (int i = 0; i < n; i ++) {
        lambdas.push_back(1/ double(n));
    }
    return lambdas;
}

std::vector<double> fluid_lambda(const std::vector<Vector>& points, double mf, double ma) {
    double sum = mf + ma;
    int n = int(points.size());
    std::vector<double> lambdas(n + 1, mf / double(n) / sum);
    lambdas[n] = ma / sum;
    return lambdas;
}

std::vector<double> normal_lambda(const std::vector<Vector>& points) {
    Vector center(0.5, 0.5);
    std::vector<double> lambdas;
    double sum;
    for (int i = 0; i < int(points.size()); i ++) {
        double l = exp(-norm(points[i] - center)*norm(points[i] - center)/0.02);
        lambdas.push_back(l);
        sum += l;
    }
    for (int i = 0; i < int(points.size()); i ++) {
        lambdas[i] /= sum;
    }
    return lambdas;
}

double g_func(const std::vector<Vector>& points, const std::vector<double>& weights, const std::vector<double>& lambdas, const double f = 1) {
    // function g
    double sum = 0;
    int n = int(points.size());
    std::vector<Polygon> cells = powerDiagram(points, weights);
    double areaSum = 0;
    for (int i = 1; i < n; i ++) {
        Polygon cell = cells[i];
        double area = polygonArea(cell);
        areaSum += area;
        sum += f * (integral(cell, points[i]) - weights[i] * area) + lambdas[i] * weights[i];
    }
    if (n != int(weights.size())) {
        sum += (lambdas[n] - areaSum) * weights[n]; 
    }
    return sum;
}

double g_grad(const Vector& point, const Polygon cell, const double lambda, const double f = 1) {
    double area = polygonArea(cell);
    return - f * area + lambda;
}
