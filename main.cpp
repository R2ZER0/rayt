#include <iostream>
#include "vec3.hpp"
#include "ray.hpp"

vec3 calc_colour(const ray& r) {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

struct imagedata {
        int xs, ys;
};


void print_imagedata(imagedata& img) {
        vec3 lower_left_corner(-2.0, -1.0, -1.0);
        vec3 horizontal(4.0, 0.0, 0.0);
        vec3 vertical(0.0, 2.0, 0.0);
        vec3 origin(0.0, 0.0, 0.0);

        std::cout << "P3\n" << img.xs << ' ' << img.ys << "\n255\n";

        for(int j = img.ys; j >= 0; --j) {
                std::cout << "# Line " << img.ys-j << '\n';

                for(int i = 0; i < img.xs; ++i) {
                        float u = float(i) / float(img.xs);
                        float v = float(j) / float(img.ys);

                        ray r(origin, lower_left_corner + u*horizontal + v*vertical);
                        vec3 colour = calc_colour(r);

                        int ir = int(255.99 * colour.r());
                        int ig = int(255.99 * colour.g());
                        int ib = int(255.99 * colour.b());

                        std::cout << ir << ' ' << ig << ' ' << ib << '\n';
                }
        }
}

int main() {
        imagedata image = { 800, 600 };
        print_imagedata(image);
}
