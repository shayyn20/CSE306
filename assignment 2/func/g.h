#include "pd.h"
#include <string>

double polygonArea(Polygon polygon) {
    double area = 0; 
    int n = int(polygon.vertices.size());
    for (int i = 0; i < n; i++) {
        area += (polygon.vertices[(i>0)?(i-1):(n - 1)][0] + polygon.vertices[i][0]) * 
                (polygon.vertices[(i>0)?(i-1):(n - 1)][1] - polygon.vertices[i][1]); 
    } 
    return abs(area / 2); 
}

double integral(Polygon cell, Vector Pi) {
    // compute the value of int_{powW(Pi)} |P - Pi|^2 dP
    double sum = 0;
    for (int k = 1; k < int(cell.vertices.size()); k ++) {
        double Xk = cell.vertices[k][0];
        double Yk = cell.vertices[k][1];
        double Xk_1 = cell.vertices[k-1][0];
        double Yk_1 = cell.vertices[k-1][1];
        sum += (Xk_1 * Yk - Xk * Yk_1) * (Xk_1*Xk_1 + Xk_1*Xk + Xk*Xk + Yk_1*Yk_1 + Yk_1*Yk + Yk*Yk
                - 4 * (Pi[0]*(Xk_1 + Xk) + Pi[1]*(Yk_1 + Yk)) + 6 * norm(Pi)*norm(Pi));
    }
    return sum/12;
}

double lambda(Vector point) {
    Vector center(0.5, 0.5);
    return exp(-norm(point - center)/0.02);
}

double g_func(std::vector<Vector>& points, std::vector<double>& weights, double f = 1) {
    // function g
    double sum = 0;
    std::vector<Polygon> cells = powerDiagram(points, weights);
    for (int i = 1; i < int(points.size()); i ++) {
        Polygon cell = cells[i];
        double area = polygonArea(cell);
        sum += f * (integral(cell, points[i]) - weights[i] * area) + lambda(points[i]) * weights[i];
    }
    return sum;
}

double g_grad(Vector& point, Polygon cell, double f = 1) {
    double area = polygonArea(cell);
    return - f * area + lambda(point);
}
