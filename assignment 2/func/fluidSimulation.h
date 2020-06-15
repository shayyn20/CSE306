#include "gradientAscent.h"

Generator generator;
Vector Fg(0, 0.1);

std::vector<Particle> init_fluid(int N = 80, double mass = 0.4) {
    std::vector<Particle> fluid;
    std::vector<Vector> pos = generateRandomPoints(N, generator);
    for (int i = 0; i < N; i ++) {
        Vector v;
        fluid.push_back(Particle(pos[i], v, mass / N));
    }
    return fluid;
}

std::vector<Particle> init_air(int M = 120, double mass = 0.6) {
    std::vector<Particle> air;
    std::vector<Vector> pos = generateRandomPoints(M, generator, false);
    pos = lloydIteration_vor(pos);
    for (int i = 0; i < M; i ++) {
        Vector v;
        air.push_back(Particle(pos[i], v, mass / M));
    }
    return air;
}

std::vector<Vector> get_position(std::vector<Particle> particles) {
    std::vector<Vector> pos;
    for (int i = 0; i < int(particles.size()); i ++) {
        pos.push_back(particles[i].position);
    }
    return pos;
}

std::vector<Vector> get_velocity(std::vector<Particle> particles) {
    std::vector<Vector> v;
    for (int i = 0; i < int(particles.size()); i ++) {
        v.push_back(particles[i].velocity);
    }
    return v;
}

std::vector<double> get_mass(std::vector<Particle> particles) {
    std::vector<double> m;
    for (int i = 0; i < int(particles.size()); i ++) {
        m.push_back(particles[i].mass);
    }
    return m;
}

std::vector<Particle> retrieve_particles(std::vector<Vector> pos, std::vector<Vector> v, std::vector<double> m) {
    std::vector<Particle> particles;
    for (int i = 0; i < int(pos.size()); i ++) {
        Particle p(pos[i], v[i], m[i]);
        particles.push_back(p);
    }
    return particles;
}

std::vector<Particle> gallouetMerigot(std::vector<Particle> particles, double dt, double eps) {
    std::vector<Vector> pos = get_position(particles);
    std::vector<Vector> v = get_velocity(particles);
    std::vector<double> m = get_mass(particles);

    std::vector<double> weights = optimalTransport(pos, 1, "uniform"); // optimize weights W of the Laguerre’s cells of allparticles
    std::vector<Polygon> cells = powerDiagram(pos, weights);
    for (int i = 0; i < int(particles.size()); i ++) {
        Vector Fspr = (centroid(cells[i]) - pos[i]) / (eps * eps);
        Vector F = Fspr + Fg;
        v[i] = v[i] + dt * F / m[i];
        pos[i] = pos[i] + dt * v[i];

        if (!isInRectangle(pos[i])) { // if the particle goes out of the box
            if (pos[i][0] < 0) {
                pos[i][0] = - pos[i][0];
                v[i][0] = - v[i][0];
            }
            if (pos[i][0] > 1) {
                pos[i][0] = 2 - pos[i][0];
                v[i][0] = - v[i][0];
            }
            if (pos[i][1] < 0) {
                pos[i][1] = - pos[i][1];
                v[i][1] = - v[i][1];
            }
            if (pos[i][1] > 1) {
                pos[i][1] = 2 - pos[i][1];
                v[i][1] = - v[i][1];
            }
        }
    }
    return retrieve_particles(pos, v, m);
}

void simulation(double dt = 0.002, double eps = 0.004) {
    std::vector<Particle> fluid = init_fluid();
    // std::vector<Particle> air = init_air();

    std::vector<Vector> pos = get_position(fluid);
    //std::vector<double> weights = optimalTransport(pos, 1, "uniform");
    //std::vector<Polygon> p = powerDiagram(pos, weights);
    std::vector<Polygon> p = voronoiParalLinEnum(pos);
    save_svg_with_point(p, pos, "fluid.svg");

    std::vector<Particle> nextFrame = gallouetMerigot(fluid, dt, eps);
    pos = get_position(nextFrame);
    //weights = optimalTransport(pos, 1, "uniform");
    //p = powerDiagram(pos, weights);
    p = voronoiParalLinEnum(pos);
    save_svg_with_point(p, pos, "fluid2.svg");
}


