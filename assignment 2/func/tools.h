#include "svg.h"

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

Vector centroid(Polygon cell) {
    double A = polygonArea(cell) + 1e-6; // avoid empty
    int n = int(cell.vertices.size());
    double cx = 0;
    double cy = 0;
    for (int i = 0; i < n; i ++) {
        cx += (cell.vertices[(i>0)?(i-1):(n - 1)][0] + cell.vertices[i][0]) * (cell.vertices[(i>0)?(i-1):(n - 1)][0] * cell.vertices[i][1] - cell.vertices[i][0] * cell.vertices[(i>0)?(i-1):(n - 1)][1]);
        cy += (cell.vertices[(i>0)?(i-1):(n - 1)][1] + cell.vertices[i][1]) * (cell.vertices[(i>0)?(i-1):(n - 1)][0] * cell.vertices[i][1] - cell.vertices[i][0] * cell.vertices[(i>0)?(i-1):(n - 1)][1]);
    }
    cx /= (6 * A);
    cy /= (6 * A);
    return Vector(cx, cy);
}