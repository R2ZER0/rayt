#include <iostream>
#include <cfloat>

// Platform
//#define RAYT_ARDUINO
#define RAYT_LINUX

// Settings
#define RAYT_IMAGE_WIDTH (1000)
#define RAYT_IMAGE_HEIGHT (500)
#define RAYT_DEPTH_LIMIT (3)
#define RAYT_NO_MULTISAMPLE
#define RAYT_DETERMINISTIC
#define RAYT_NO_OPENMP

#include "vec3.hpp"
#include "ray.hpp"
#include "hitable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "lambertian.hpp"
#include "metal.hpp"
#include "dielectric.hpp"
#include "random.hpp"

/* Potential Improvements:
 * - Make it deterministic (and maybe not require AA?)
 * - Ray splitting, instead of more rays (allow scatter to return multiple rays/attenuations)
 * - Directional "sky" light source
 * - Point ligth source
 * - Triangles
 * - Parallelism
 * - HDR???
 * - More options to dielectrics, like transparency, colour
 * - Textures
 */


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
        vec3* colourdata;
};

void render_to_imagedata(hitable* world, imagedata& img) {
    const camera cam;

    #ifdef RAYT_OPENMP
    #pragma omp parallel for
    #endif
    for(int j = 0; j < img.ys; ++j) {
            #ifdef RAYT_OPENMP
            #pragma omp parallel for
            #endif
            for(int i = 0; i < img.xs; ++i) {

                    vec3 colour(0.0, 0.0, 0.0);
                    
                    #ifdef RAYT_MULTISAMPLE
                        for(int s = 0; s < ns; ++s) {

                            float u = float(i + get_rand()) / float(img.xs);
                            float v = float(j + get_rand()) / float(img.ys);

                            ray r = cam.get_ray(u, v);
                            colour += calc_colour(r, world, 0);
                        }

                        // correct for number of samples
                        colour /= float(ns);

                    #else
                        float u = float(i + 0.5) / float(img.xs);
                        float v = float(j + 0.5) / float(img.ys);

                        ray r = cam.get_ray(u, v);
                        colour = calc_colour(r, world, 0); 
                    #endif

                    // Gammar correction
                    colour = vec3( sqrt(colour[0]), sqrt(colour[1]), sqrt(colour[2]) );

                    int idx = (img.ys-j-1)*img.xs + i;
                    if(idx > img.xs*img.ys || idx < 0) {
                        #ifdef RAYT_LINUX
                        std::cerr << "Index out of range! " << idx << "i,j = " << i << ',' << j << '\n';
                        #endif
                    } else {
                        img.colourdata[idx] = colour;
                    }
                    
            }
    }
}

#ifdef RAYT_LINUX
void print_imagedata(imagedata& img) {
    #ifdef RAYT_MULTISAMPLE
        const int ns = 100;
    #endif

    std::cout << "P3\n" << img.xs << ' ' << img.ys << "\n255\n";
    for(int p = 0; p < img.xs*img.ys; ++p) {
        vec3 colour = img.colourdata[p];

        // convert colour to integers for output
        int ir = int(255.99 * colour.r());
        int ig = int(255.99 * colour.g());
        int ib = int(255.99 * colour.b());

        std::cout << ir << ' ' << ig << ' ' << ib << '\n';
    }
}
#endif


// Global scene data
vec3 colourdata[RAYT_IMAGE_WIDTH * RAYT_IMAGE_HEIGHT];
imagedata image = { RAYT_IMAGE_WIDTH, RAYT_IMAGE_HEIGHT, &colourdata[0] };

lambertian sphere1_material(vec3(0.8, 0.3, 0.3));
sphere sphere1_object(vec3(0,0.0,-1), 0.5, &sphere1_material);

lambertian sphere2_material(vec3(0.8, 0.8, 0.0));
sphere sphere2_object(vec3(0,-100.5,-1), 100, &sphere2_material);

metal sphere3_material(vec3(0, 0.6, 0.2), 0.3);
sphere sphere3_object(vec3(1,0,-1), 0.5, &sphere3_material);

metal sphere4_material(vec3(0.9, 0.9, 0.9), 0.0);
sphere sphere4_object(vec3(-1,0,-1), 0.5, &sphere4_material);

hitable* scene_objects[4] = {
    &sphere1_object,
    &sphere2_object,
    &sphere3_object,
    &sphere4_object
};

hitable_list world(scene_objects, 4);

#ifdef RAYT_LINUX
int main() {
    render_to_imagedata(&world, image);
    print_imagedata(image);
}
#endif