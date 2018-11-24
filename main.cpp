#include <iostream>
#include "vec3.hpp"
#include "ray.hpp"
#include <cfloat>
#include "hitable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "random.hpp"

#define DEPTH_LIMIT 50

vec3 calc_colour(const ray& r, hitable* world, int depth) {
    hit_record rec;
    if(world->hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;

        if(depth < 50 && rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation * calc_colour(scattered, world, depth+1);
        } else {
            return vec3(0, 0, 0);
        } 
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
        int ns = 100;
        bool multisample = true;

        hitable* list[4];
        list[0] = new sphere(vec3(0,0.0,-1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
        list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
        list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0, 0.6, 0.2))); 
        list[3] = new sphere(vec3(-1,0,-1), 0.5, new metal(vec3(0.8, 0.8, 0.8)));
        hitable* world = new hitable_list(list, 4);

        camera cam;

        std::cout << "P3\n" << img.xs << ' ' << img.ys << "\n255\n";

        for(int j = img.ys; j >= 0; --j) {
                std::cout << "# Line " << img.ys-j << '\n';

                for(int i = 0; i < img.xs; ++i) {

                        vec3 colour(0.0, 0.0, 0.0);
                       
                        if(multisample) { 
                            for(int s = 0; s < ns; ++s) {

                                float u = float(i + get_rand()) / float(img.xs);
                                float v = float(j + get_rand()) / float(img.ys);

                                ray r = cam.get_ray(u, v);
                                colour += calc_colour(r, world, 0);
                            }

                            // correct for number of samples
                            colour /= float(ns);

                        } else {
                            float u = float(i + 0.5) / float(img.xs);
                            float v = float(j + 0.5) / float(img.ys);

                            ray r = cam.get_ray(u, v);
                            colour = calc_colour(r, world, 0); 
                        }

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
