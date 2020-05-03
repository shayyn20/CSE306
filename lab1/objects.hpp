#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>
#include "vector.hpp"

using namespace std;

struct Intersection {
    bool exists;
    double dist;
    Vector point;
    Vector normal;
    int index;
};

struct Camera {
    Vector center;
    double distance;
};

struct Image {
    int width;
    int height;
    uint8_t* pixels;
};

class Ray {
public: 
    Ray(Vector o, Vector u);
    Vector origin;
    Vector unit;
};

class Sphere {
public: 
    Sphere();
    Sphere(Vector c, double r, Vector clr, string t);
    Intersection intersect(Ray* ray);
    Vector albedo;
    string type;
    Vector center;
    double radius;
};

class Light: public Sphere {
public:
    Light();
    Light(Vector c, double r, double intensity);
    double intensity;
};

class Scene {
public: 
    Scene(int W, int H, double f);
    Sphere* sphere_collection;
    Image image;

    Intersection closest(Ray* ray);
    Vector get_color(Ray* ray, int ray_depth);
    void set_color(int x, int y, Vector color);
    Image scan_center();
    Image scan();

    Camera camera;
    Light light;
    Sphere center_sphere;
    Sphere left_Sphere;
    Sphere right_Sphere;
    Sphere wall_red;
    Sphere wall_gre;
    Sphere wall_blu;
    Sphere wall_pin;
    Sphere wall_cya;
    Sphere wall_yel;
};