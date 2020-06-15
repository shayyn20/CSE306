#include "include.h"

int main(int argc, char** argv) {
    if ((argc > 1) && (!strcmp(argv[1], "sh"))) {
        Polygon p = Polygon({   Vector(0.05, 0.15), Vector(0.2, 0.05), Vector(0.35, 0.15), Vector(0.35, 0.3), 
                                Vector(0.25, 0.3), Vector(0.2, 0.25), Vector(0.15, 0.35), Vector(0.1, 0.25),
                                Vector(0.1, 0.2)});

        Polygon c = Polygon({Vector(0.1, 0.1), Vector(0.3, 0.1), Vector(0.3, 0.3), Vector(0.1, 0.3)});

        save_svg({p}, "test.svg");
        save_svg({c}, "test1.svg");
        save_svg({p, c}, "test3.svg");
        p = sutherlandHodgman(p, c);
        save_svg({p}, "test2.svg");
        return 0;
    }
    if ((argc > 1) && (!strcmp(argv[1], "t"))) {

        std::vector<Vector> points = {  Vector(0.05, 0.15), Vector(0.2, 0.05), Vector(0.35, 0.15), Vector(0.35, 0.3), 
                                        Vector(0.25, 0.3), Vector(0.2, 0.25), Vector(0.15, 0.35), Vector(0.1, 0.25),
                                        Vector(0.1, 0.2)};
        std::vector<double> weights = {                0.01,               0.01,                0.01,               0.01,
                                                       0.01,               0.01,                0.01,               0.01,
                                                       0.01};
        
        return 0;
    }
    if ((argc > 1) && (!strcmp(argv[1], "vor"))) {
        // std::vector<Vector> points = {  Vector(0.05, 0.15), Vector(0.2, 0.05), Vector(0.35, 0.15), Vector(0.35, 0.3), 
        //                                 Vector(0.25, 0.3), Vector(0.2, 0.25), Vector(0.15, 0.35), Vector(0.1, 0.25),
        //                                 Vector(0.1, 0.2)};
        Generator generator;
        std::vector<Vector> points = generateRandomPoints(50, generator);
        std::vector<Polygon> p = voronoiParalLinEnum(points);
        save_svg_with_point(p, points, "vor.svg");
        return 0;
    }
    if ((argc > 1) && (!strcmp(argv[1], "pow"))) {
        // std::vector<Vector> points = {  Vector(0.05, 0.15), Vector(0.2, 0.05), Vector(0.35, 0.15), Vector(0.35, 0.3), 
        //                                 Vector(0.25, 0.3), Vector(0.2, 0.25), Vector(0.15, 0.35), Vector(0.1, 0.25),
        //                                 Vector(0.1, 0.2)};
        // std::vector<double> weights = {                0.01,               0.02,                0.03,               0.04,
        //                                                0.05,               0.04,                0.03,               0.02,
        //                                                0.01};
        Generator generator;
        std::vector<Vector> points = generateRandomPoints(50, generator);
        std::vector<double> weights = generateRandomWeights(50, generator);
        std::vector<Polygon> p = powerDiagram(points, weights);
        save_svg_with_point(p, points, "power.svg");
        return 0;
    }
    if ((argc > 1) && (!strcmp(argv[1], "ga"))) {
        std::vector<Vector> points = {  Vector(0.05, 0.15), Vector(0.2, 0.05), Vector(0.35, 0.15), Vector(0.35, 0.3), 
                                        Vector(0.25, 0.3), Vector(0.2, 0.25), Vector(0.15, 0.35), Vector(0.1, 0.25),
                                        Vector(0.1, 0.2)};
        std::vector<double> weights = {             0.01,               0.01,                0.01,               0.01,
                                                    0.01,               0.01,                0.01,               0.01,
                                                    0.01};
        std::vector<double> nw = gradAscent(points, weights, 0.6);
        std::vector<Polygon> p = powerDiagram(points, nw);
        for (int i = 0; i < int(nw.size()); i ++) {
            printf("w[%d] = %f \n", i, nw[i]);
        }
        save_svg(p, "ga.svg");
        p.push_back(Polygon(points));
        save_svg(p, "ga2.svg");
        p = voronoiParalLinEnum(points);
        save_svg(p, "ga1.svg");
        return 0;
    }
    
    // std::vector<Vector> points = {  Vector(0.05, 0.15), Vector(0.2, 0.05), Vector(0.35, 0.15), Vector(0.35, 0.3), 
    //                                 Vector(0.25, 0.3), Vector(0.2, 0.25), Vector(0.15, 0.35), Vector(0.1, 0.25),
    //                                 Vector(0.1, 0.2)};

    Generator generator;
    std::vector<Vector> points = generateRandomPoints(500, generator, false);
    std::vector<double> weights = optimalTransport(points, 1);
    std::vector<Polygon> p = voronoiParalLinEnum(points);
    save_svg_with_point(p, points, "ot.svg");
    p = powerDiagram(points, weights);
    save_svg_with_point(p, points, "ot2.svg");
    return 0;
}