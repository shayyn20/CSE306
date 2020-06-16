#include "gradientAscent.h"

Generator generator;
Vector Fg(0, -100);

std::vector<Particle> init_fluid(int N, double mass) {
    std::vector<Particle> fluid;
    std::vector<Vector> pos = generateRandomPoints(N, generator);
    for (int i = 0; i < N; i ++) {
        Vector v;
        fluid.push_back(Particle(pos[i], v, mass / N));
    }
    return fluid;
}

std::vector<Particle> init_air(int M, double mass) {
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
        // printf("Fspr = (%f, %f), F = (%f, %f) \n", Fspr[0], Fspr[1], F[0], F[1]);
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
            if (pos[i][1] <= 0) {
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

void simulation(double dt = 0.0005, double eps = 0.04, int N = 80, double mf = 0.4, int M = 120, double ma = 0.6) {
    std::vector<Particle> fluid = init_fluid(N, mf);
    std::vector<Particle> air = init_air(M, ma);

    std::vector<Vector> fpos = get_position(fluid);
    std::vector<Vector> apos = get_position(air);

    std::vector<double> lambdas = fluid_lambda(fpos, mf, ma);

    ParticleWeight pw = optimalTransport_fluid(fpos, lambdas, 1);
    std::vector<double> wf = pw.fluid;
    std::vector<double> wa(int(air.size()), pw.air/double(air.size()));

    apos = lloydIteration_pow(apos, wa);
    std::vector<Vector> pos = concatenate(fpos, apos); 
    std::vector<double> w = concatenate(wf, wa);

    std::vector<Polygon> p = powerDiagram(pos, w);

    // std::vector<Polygon> pf = powerDiagram(fpos, wf);
    // apos = lloydIteration_pow(apos, wa);
    // std::vector<Polygon> pa = powerDiagram(apos, wa);

    save_svg_animated_with_point(p, pos, "fluid.svg", 0, 10);

    // save_frame(p, "fluid", 0);

    for (int i = 1; i < 10; i ++) {
        fluid = gallouetMerigot(fluid, dt, eps);
        fpos = get_position(fluid);
        apos = get_position(air);

        pw = optimalTransport_fluid(fpos, lambdas, 1);
        wf = pw.fluid;
        wa = std::vector<double>(int(air.size()), pw.air/double(air.size()));

        apos = lloydIteration_pow(apos, wa);

        pos = concatenate(fpos, apos); 
        w = concatenate(wf, wa);

        p = powerDiagram(pos, w);

        // std::vector<Polygon> pf = powerDiagram(fpos, wf);
        // apos = lloydIteration_pow(apos, wa);
        // std::vector<Polygon> pa = powerDiagram(apos, wa);

        save_svg_animated_with_point(p, pos, "fluid.svg", i, 10);
    }
}


