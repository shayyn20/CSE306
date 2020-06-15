#include "powerDiagram.h"
#include <string>

double polygonArea(const Polygon polygon) {
    double area = 0; 
    const int n = int(polygon.vertices.size());
    for (int i = 0; i < n; i++) {
        area += (polygon.vertices[(i>0)?(i-1):(n - 1)][0] + polygon.vertices[i][0]) * 
                (polygon.vertices[(i>0)?(i-1):(n - 1)][1] - polygon.vertices[i][1]); 
    } 
    return abs(area / 2); 
}

double integral(const Polygon cell, const Vector Pi) {
    // compute the value of int_{powW(Pi)} |P - Pi|^2 dP
    double sum = 0;
    const int n = int(cell.vertices.size());
    for (int k = 0; k < n; k ++) {
        double Xk = cell.vertices[k][0];
        double Yk = cell.vertices[k][1];
        double Xk_1 = cell.vertices[(k>0)?(k-1):(n - 1)][0];
        double Yk_1 = cell.vertices[(k>0)?(k-1):(n - 1)][1];
        sum += (Xk_1 * Yk - Xk * Yk_1) * (Xk_1*Xk_1 + Xk_1*Xk + Xk*Xk + Yk_1*Yk_1 + Yk_1*Yk + Yk*Yk
                - 4 * (Pi[0]*(Xk_1 + Xk) + Pi[1]*(Yk_1 + Yk)) + 6 * norm(Pi)*norm(Pi));
    }
    return sum/12;
}

std::vector<double> uniform_lambda(const std::vector<Vector>& points) {
    std::vector<double> lambdas;
    int n = int(points.size());
    for (int i = 0; i < n; i ++) {
        lambdas.push_back(1 / n);
    }
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
    std::vector<Polygon> cells = powerDiagram(points, weights);
    for (int i = 1; i < int(points.size()); i ++) {
        Polygon cell = cells[i];
        double area = polygonArea(cell);
        sum += f * (integral(cell, points[i]) - weights[i] * area) + lambdas[i] * weights[i];
    }
    return sum;
}

double g_grad(const Vector& point, const Polygon cell, const double lambda, const double f = 1) {
    double area = polygonArea(cell);
    return - f * area + lambda;
}
