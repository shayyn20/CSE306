#include "ga.h"

Generator generator;

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

// std::vector<Particle> gallouetMerigot(std::vector<Particle> particles, double dt) {
//     std::vector<Vector> pos;
//     std::vector<Vector> v;
//     std::vector<double> m;
//     for (int i = 0; i < int(particles.size()); i ++) {
//         pos.push_back(particles[i].position);
//         v.push_back(particles[i].velocity);
//         m.push_back(particles[i].mass);
//     }
//     std::vector<double> weights = optimalTransport(pos, 1); // optimize weights W of the Laguerre’s cells of allparticles
//     for (int i = 0; i < int(particles.size()); i ++) {
//     }
// 
// 
// }

