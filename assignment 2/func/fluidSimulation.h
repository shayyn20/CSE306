#include "gradientAscent.h"

Generator generator;
Vector Fg(0, 1);

std::vector<Particle> init_fluid(int N = 700, double mass = 700) {
    std::vector<Particle> fluid;
    std::vector<Vector> pos = generateRandomPoints(N, generator);
    for (int i = 0; i < N; i ++) {
        Vector v;
        fluid.push_back(Particle(pos[i], v, mass / N));
    }
    return fluid;
}

std::vector<Particle> init_air(int M = 2500, double mass = 2500) {
    std::vector<Particle> air;
    std::vector<Vector> pos = generateRandomPoints(M, generator, false);
    for (int i = 0; i < M; i ++) {
        Vector v;
        air.push_back(Particle(pos[i], v, mass / M));
    }
    return air;
}

Vector centroid(Polygon cell) {
    double A = polygonArea(cell);
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

std::vector<Particle> gallouetMerigot(std::vector<Particle> particles, double dt, double eps) {
    std::vector<Vector> pos;
    std::vector<Vector> v;
    std::vector<double> m;

    for (int i = 0; i < int(particles.size()); i ++) {
        pos.push_back(particles[i].position);
        v.push_back(particles[i].velocity);
        m.push_back(particles[i].mass);
    }
    std::vector<double> weights = optimalTransport(pos, 1); // optimize weights W of the Laguerre’s cells of allparticles
    std::vector<Polygon> cells = powerDiagram(pos, weights);

    std::vector<Particle> res;
    for (int i = 0; i < int(particles.size()); i ++) {
        Vector Fspr = (centroid(cells[i]) - pos[i]) / (eps * eps);
        Vector F = Fspr + Fg;
        v[i] = v[i] + dt * F / m[i];
        pos[i] = pos[i] + dt * v[i];
    }
    for (int i = 0; i < int(particles.size()); i ++) {
        res[i].position = pos[i];
        res[i].velocity = v[i];
        res[i].mass = m[i];
    }

    return res;
}

