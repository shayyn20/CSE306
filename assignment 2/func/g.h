#include "pd.h"

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

double g_func(std::vector<Vector>& points, std::vector<double>& weights, double f = 1) {
    // function g
    Polygon subj = QUAD;
    double sum = 0;
    for (int i = 1; i < int(points.size()); i ++) {
        Polygon cell = sutherlandHodgman_pow(subj, points, weights, i);
        double area = polygonArea(cell);
        sum += f * (integral(cell, points[i]) - weights[i] * area) + area * weights[i];
    }
    return sum;
}

double g_grad(std::vector<Vector>& points, int i, std::vector<double>& weights, double f = 1) {
    Polygon subj = QUAD;
    Polygon cell = sutherlandHodgman_pow(subj, points, weights, i);
    //for (int i = 0; i < int(cell.vertices.size()); i++) {
    //    printf("(%f, %f) \n", cell.vertices[i][0], cell.vertices[i][1]);
    //}
    double area = polygonArea(cell);
    //printf("area: %f \n", area);
    return - f * area + area;
}
