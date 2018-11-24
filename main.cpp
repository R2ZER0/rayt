#include <iostream>
#include "vec3.hpp"
#include "ray.hpp"
#include <cfloat>
#include "hitable_list.hpp"
#include "sphere.hpp"

vec3 calc_colour(const ray& r, hitable* world) {
    hit_record rec;
    if(world->hit(r, 0.0, FLT_MAX, rec)) {
        return 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
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
        vec3 lower_left_corner(-2.0, -1.0, -1.0);
        vec3 horizontal(4.0, 0.0, 0.0);
        vec3 vertical(0.0, 2.0, 0.0);
        vec3 origin(0.0, 0.0, 0.0);

        hitable* list[2];
        list[0] = new sphere(vec3(0,0,-1), 0.5);
        list[1] = new sphere(vec3(0,-100.5,-1), 100);
        hitable* world = new hitable_list(list, 2);

        std::cout << "P3\n" << img.xs << ' ' << img.ys << "\n255\n";

        for(int j = img.ys; j >= 0; --j) {
                std::cout << "# Line " << img.ys-j << '\n';

                for(int i = 0; i < img.xs; ++i) {
                        float u = float(i) / float(img.xs);
                        float v = float(j) / float(img.ys);

                        ray r(origin, lower_left_corner + u*horizontal + v*vertical);
                        vec3 colour = calc_colour(r, world);

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
