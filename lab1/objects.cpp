#include <stdio.h>
#include <stdlib.h>
#include "objects.hpp"

Ray::Ray(Vector o, Vector u) {
    this->origin = o;
    this->unit = u;
}

Sphere::Sphere() {
}

Sphere::Sphere(Vector c, double r, Vector clr, string t) {
    this->center = c;
    this->radius = r;
    this->albedo = clr;
    this->type = t;
}

Light::Light() {
}

Light::Light(Vector c, double r, double intensity = 100000) {
    this->center = c;
    this->radius = r;
    this->albedo = Vector(1, 1, 1);
    this->type = "light";
    this->intensity = intensity;
}

Intersection Sphere::intersect(Ray* ray) {
    Intersection i;
    Vector O = ray->origin;
    Vector U = ray->unit;
    Vector C = this->center;
    double R = this->radius;
    double delta = pow(dot(U, O-C),2) - pow((norm(O-C)),2) + pow(R,2);
    if (delta < 0) {
        i.exists = false;
    }
    else {
        double t_1 = dot(U, C-O) - sqrt(delta);
        double t_2 = dot(U, C-O) + sqrt(delta);
        double t;
        if (t_2 < 0) {
            i.exists = false;
        }
        else {
            i.exists = true;
            if (t_1 >= 0) {
                t = t_1;
            }
            else {
                t = t_2;
            }
            i.dist = t;
            i.point = O + (U * t);
            i.index = -1;
            i.normal = normalize(i.point - C);
        }
    }
    return i;
}

Scene::Scene(int W, int H, double f) {
    this->image.width = W;
    this->image.height = H;
    uint8_t* temp_img = (uint8_t*)malloc(W*H*3 * sizeof(uint8_t));
    this->image.pixels = temp_img;
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

    static Sphere collection[] = {this->center_sphere, this->left_Sphere, this->right_Sphere, this->wall_red, this->wall_gre, this->wall_blu, 
                                this->wall_pin, this->wall_cya, this->wall_yel, this->light};
    this->sphere_collection = collection;
}

Intersection Scene::closest(Ray* ray) {
    double min = INFINITY;
    Intersection closest;
    for (int i = 0; i < sizeof(*this->sphere_collection)/sizeof(Sphere*); i++) {
        Sphere cur = this->sphere_collection[i];
        Intersection cur_i = cur.intersect(ray);
        if (cur_i.exists && cur_i.dist < min) {
            min = cur_i.dist;
            closest = cur_i;
            closest.index = i;
        }
    }
    return closest;
}

Vector Scene::get_color(Ray* ray, int ray_depth) {
    if (ray_depth < 0) {
        //if (this->sphere_collection[this->closest(ray).index].type == "transparent" && ray_depth < 1) {
        //    printf("shit \n");
        //}
        return Vector(0, 0, 0);
    }

    Intersection inter = this->closest(ray);
    int id = inter.index;
    Sphere obj = this->sphere_collection[id];
    Vector color = obj.albedo;

    if (id > -1) {
        Vector S = this->light.center;
        Vector N = inter.normal;
        Vector P = inter.point + (N * 0.001);
        Vector wi = ray->unit;
        double I = this->light.intensity;

        if (obj.type == "diffuse") {
            //direct lighting
            double d = norm(S - P);
            Vector w = normalize(S - P);
            Ray test_ray(P, w);
        
            int visible;
            Intersection test_visible = this->closest(&test_ray);
            if (this->sphere_collection[test_visible.index].type == "light" || !test_visible.exists) {
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
            color = (obj.albedo / M_PI) * (I / (4 * M_PI * pow(d, 2))) * visible * max(dot(N, w), 0.);
        }

        if (obj.type == "mirror") {
            Vector w = wi - N * (2 * dot(N, wi));
            Ray refl = Ray(P, w);
            color = get_color(&refl, ray_depth - 1);
        }

        if (obj.type == "transparent") {   
            double n1 = 1;
            double n2 = 1.5;
            Vector P = inter.point - (N * 0.001);

            double radicand = 1 - (pow((n1/n2), 2) * (1 - pow(dot(N, wi), 2)));

            // Fresnel law
            //double k0 = pow(((n1 - n2)/(n1 + n2)), 2);
            //double R = k0 + (1 - k0) * pow((1 - abs(dot(N, wi))), 5);
            //double ran = (double) rand() / (RAND_MAX);
            //
            //if (ran < R) {
            //    Vector w = wi - N * (2 * dot(N, wi));
            //    Ray refl = Ray(P, w);
            //    color = get_color(&refl, ray_depth - 1);
            //}
            //
            //else {
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
            //}
        }
    }
    return color;
}

void Scene::set_color(int x, int y, Vector color) {
    double gamma = 2.2;
    this->image.pixels[y * this->image.width * 3 + x*3 + 0] = min(255., pow(color[0], 1/gamma)*255);
    this->image.pixels[y * this->image.width * 3 + x*3 + 1] = min(255., pow(color[1], 1/gamma)*255);
    this->image.pixels[y * this->image.width * 3 + x*3 + 2] = min(255., pow(color[2], 1/gamma)*255);
}

Image Scene::scan_center() {
    int W = this->image.width;
    int H = this->image.height;
    double f = this->camera.distance;
    double Qx = this->camera.center[0];
    double Qy = this->camera.center[1];
    double Qz = this->camera.center[2];
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            int x = j;
            int y = H-i-1;
            Vector ray_direct(Qx + x + 0.5 - W/2, Qy + y + 0.5 - H/2, Qz - f);
            Vector ray_unit = normalize(ray_direct - this->camera.center);
            Ray ray(this->camera.center, ray_unit);
            Intersection intsect = this->center_sphere.intersect(&ray);
            if (intsect.exists) {
                Vector color = this->center_sphere.albedo;
                set_color(x, H-y-1, color);
            }
        }
    }
    return this->image;
}

Image Scene::scan() {
    int W = this->image.width;
    int H = this->image.height;
    double f = this->camera.distance;
    double Qx = this->camera.center[0];
    double Qy = this->camera.center[1];
    double Qz = this->camera.center[2];
    #pragma omp parallel for, schedule(dynamic, 1)
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            int x = j;
            int y = i;
            Vector ray_direct(Qx + x + 0.5 - W/2, Qy + y + 0.5 - H/2, Qz - f);
            Vector ray_unit = normalize(ray_direct - this->camera.center);
            Ray ray(this->camera.center, ray_unit);
            Vector color = get_color(&ray, 2);
            set_color(x, H-y-1, color);
        }
    }
    return this->image;
}