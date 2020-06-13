#ifndef SCENE
#define SCENE

#include <string>
#include "mesh.h"
#include "../function/functions.h"

using namespace std;


class Scene {
public: 
    vector<Geometry*> collection;
    Image image;
    Camera camera;

    Light light;
    Sphere center_sphere;
    Sphere left_Sphere;
    Sphere right_Sphere;
    TriangleMesh cat;
    Sphere wall_red;
    Sphere wall_gre;
    Sphere wall_blu;
    Sphere wall_pin;
    Sphere wall_cya;
    Sphere wall_yel;

    Scene(int W, int H, double f) {
        this->image.width = W;
        this->image.height = H;
        this->image.pixels = vector<uint8_t>(W*H*3);
        this->camera.center = Vector(0, 0, 55);
        this->camera.distance = f;

        this->light = Light(Vector(-10, 20, 40), 5);
        this->center_sphere = Sphere(Vector(0, 0, 0), 10, Vector(1, 1, 1), "diffuse");
        this->left_Sphere = Sphere(Vector(-20, 0, 0), 10, Vector(1, 1, 1), "mirror");
        this->right_Sphere = Sphere(Vector(20, 0, 0), 10, Vector(1, 1, 1), "transparent");
        this->wall_red = Sphere(Vector(0,  1000, 0), 940, Vector(1, 0, 0), "diffuse");
        this->wall_gre = Sphere(Vector(0, 0, -1000), 940, Vector(0, 1, 0), "diffuse");
        this->wall_blu = Sphere(Vector(0, -1000, 0), 990, Vector(0, 0, 1), "diffuse");
        this->wall_pin = Sphere(Vector(0, 0,  1000), 940, Vector(1, 0, 1), "diffuse");
        this->wall_cya = Sphere(Vector( 1000, 0, 0), 940, Vector(0, 1, 1), "diffuse");
        this->wall_yel = Sphere(Vector(-1000, 0, 0), 940, Vector(1, 1, 0), "diffuse");

        this->cat = TriangleMesh(Vector(1, 1, 1), "diffuse");
        this->cat.readOBJ("./model/cat/cat.obj");
        for (int i = 0; i < int(this->cat.vertices.size()); i++) {
            this->cat.vertices[i] = 0.6 * this->cat.vertices[i] + Vector(0, 20, 0);
        }
        // &this->cat
        // &this->center_sphere, &this->left_Sphere, &this->right_Sphere 
        static Geometry* collection[] = {&this->center_sphere, &this->left_Sphere, &this->right_Sphere, &this->wall_red, &this->wall_gre, &this->wall_blu, 
                                    &this->wall_pin, &this->wall_cya, &this->wall_yel, &this->light};
        this->collection = vector<Geometry *>(collection, collection + sizeof(collection) / sizeof(collection[0]));
    }

    Intersection closest(Ray* ray) {
        double min = INFINITY;
        Intersection closest;
        for (int i = 0; i < int(this->collection.size()); i++) {
            Geometry* cur = this->collection[i];
            Intersection cur_i = cur->intersect(ray);
            if (cur_i.exists && cur_i.dist < min) {
                min = cur_i.dist;
                closest = cur_i;
                closest.index = i;
            }
        }
        return closest;
    }

    Vector get_color(Ray* ray, int ray_depth) {
        if (ray_depth < 0) {
            return Vector(0, 0, 0);
        }

        Intersection inter = this->closest(ray);
        int id = inter.index;
        Geometry* obj = this->collection[id];
        Vector color = obj->albedo;
        Vector temp_color;

        if (id > -1) {
            Vector S = this->light.center;
            Vector N = inter.normal;
            Vector P = inter.point + (N * 0.001);
            Vector wi = ray->unit;
            double I = this->light.intensity;

            if (obj->type == "diffuse") {
                //direct lighting
                double d = norm(S - P);
                Vector w = normalize(S - P);
                Ray test_ray(P, w);

                int visible;
                Intersection test_visible = this->closest(&test_ray);
                if (this->collection[test_visible.index]->type == "light" || !test_visible.exists) {
                    visible = 1;
                }
                else {
                    if (test_visible.dist > d) {
                        visible = 1;
                    }
                    else {
                        visible = 0;
                    }
                }
                color = (obj->albedo / M_PI) * (I / (4 * M_PI * pow(d, 2))) * visible * max(dot(N, w), 0.);
                temp_color = color;

                // indirect
                Ray random_ray = Ray(P, random_cos(N));
                color = temp_color + get_color(&random_ray, ray_depth - 1) * obj->albedo;
            }

            else if (obj->type == "mirror") {
                Vector w = wi - N * (2 * dot(N, wi));
                Ray refl = Ray(P, w);
                color = get_color(&refl, ray_depth - 1);
            }

            else if (obj->type == "transparent") {   
                double n1 = 1;
                double n2 = 1.5;
                Vector P = inter.point - (N * 0.001);

                double radicand = 1 - (pow((n1/n2), 2) * (1 - pow(dot(N, wi), 2)));

                // Fresnel law
                double k0 = pow(((n1 - n2)/(n1 + n2)), 2);
                double R = k0 + (1 - k0) * pow((1 - abs(dot(N, wi))), 5);
                double ran = (double) rand() / (RAND_MAX);

                if (ran < R) {
                    Vector w = wi - N * (2 * dot(N, wi));
                    Ray refl = Ray(P, w);
                    color = get_color(&refl, ray_depth - 1);
                }

                else {
                    if (radicand < 0) {
                        Vector w = wi - N * (2 * dot(N, wi));
                        Ray refl = Ray(P, w);
                        color = get_color(&refl, ray_depth - 1);
                    }
                    else {
                        if (dot(wi, N) > 0) {
                            P = inter.point + (N * 0.001);
                            N =  N * -1;
                            n1 = 1.5;
                            n2 = 1;
                        }  
                        Vector wT = (wi - N * dot(N, wi)) * (n1 / n2);
                        Vector wN = N * sqrt(radicand) * -1;
                        Vector w = wN + wT;
                        Ray refr = Ray(P, w);
                        color = get_color(&refr, ray_depth - 1);
                    }
                }
            }
        }
        return color;
    }

    void set_color(int x, int y, Vector color) {
        double gamma = 2.2;
        this->image.pixels[y * this->image.width * 3 + x*3 + 0] = min(255., pow(color[0], 1/gamma)*255);
        this->image.pixels[y * this->image.width * 3 + x*3 + 1] = min(255., pow(color[1], 1/gamma)*255);
        this->image.pixels[y * this->image.width * 3 + x*3 + 2] = min(255., pow(color[2], 1/gamma)*255);
    }

    Image scan(int NB_path, int ray_depth) {
        int W = this->image.width;
        int H = this->image.height;
        double f = this->camera.distance;
        double Qx = this->camera.center[0];
        double Qy = this->camera.center[1];
        double Qz = this->camera.center[2];
        #pragma omp parallel for schedule(dynamic, 1)
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W; ++j) {
                Vector color;
                double offset_x, offset_y;

                for (int k = 0; k < NB_path; ++k) {
                    Vector ray_direct(Qx + j + 0.5 - W/2, Qy + i + 0.5 - H/2, Qz - f);
                    
                    boxMuller(1, offset_x, offset_y);
                    ray_direct[0] += offset_x;
                    ray_direct[1] += offset_y;

                    Vector ray_unit = normalize(ray_direct - this->camera.center);
                    Ray ray(this->camera.center, ray_unit);

                    color = color + get_color(&ray, ray_depth);
                }
                color = color / NB_path;
                set_color(j, H-i-1, color);
            }
        }
        return this->image;
    }
};

#endif