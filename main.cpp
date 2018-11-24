#include <iostream>
#include "vec3.hpp"
#include "ray.hpp"
#include <cfloat>
#include "hitable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"

extern double drand48(void);

float get_rand() {
    return (float)drand48();
}

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(get_rand(), get_rand(), get_rand()) - vec3(1, 1, 1);
    } while(p.squared_length() >= 1.0);
    return p;
}

vec3 calc_colour(const ray& r, hitable* world) {
    hit_record rec;
    if(world->hit(r, 0.001, FLT_MAX, rec)) {
        // Instead of supplying a colour, bounce the ray randomly/diffusely
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5*calc_colour(ray(rec.p, target-rec.p), world);
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

struct imagedata {
        int xs, ys;
};


void print_imagedata(imagedata& img) {
        int ns = 48;

        hitable* list[2];
        list[0] = new sphere(vec3(0,0.0,-1), 0.5);
        list[1] = new sphere(vec3(0,-100.5,-1), 100);
        hitable* world = new hitable_list(list, 2);

        camera cam;

        std::cout << "P3\n" << img.xs << ' ' << img.ys << "\n255\n";

        for(int j = img.ys; j >= 0; --j) {
                std::cout << "# Line " << img.ys-j << '\n';

                for(int i = 0; i < img.xs; ++i) {

                        vec3 colour(0.0, 0.0, 0.0);
                        
                        for(int s = 0; s < ns; ++s) {

                            float u = float(i + get_rand()) / float(img.xs);
                            float v = float(j + get_rand()) / float(img.ys);

                            ray r = cam.get_ray(u, v);
                            colour += calc_colour(r, world);
                        }

                        // correct for number of samples
                        colour /= float(ns); 
                        // Gammar correction
                        colour = vec3( sqrt(colour[0]), sqrt(colour[1]), sqrt(colour[2]) );
                        
                        // convert colour to integers for output
                        int ir = int(255.99 * colour.r());
                        int ig = int(255.99 * colour.g());
                        int ib = int(255.99 * colour.b());

                        std::cout << ir << ' ' << ig << ' ' << ib << '\n';
                }
        }
}

int main() {
        imagedata image = { 800, 400 };
        print_imagedata(image);
}
