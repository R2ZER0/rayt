#ifndef _RENDER_HPP_
#define _RENDER_HPP_

#include <cfloat>

// Platform
//#define RAYT_ARDUINO
//#define RAYT_LINUX

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

// render_callback(void* userdata, int x, int y, vec3 rgb);
typedef void (*render_callback_t)(void*, int, int, vec3);

void render_pixel(hitable* world, const camera& cam, int xs, int ys, int x, int y, vec3& colour_out) {
                    
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
        float u = float(x + 0.5) / float(xs);
        float v = float(y + 0.5) / float(ys);

        ray r = cam.get_ray(u, v);
        colour_out = calc_colour(r, world, 0); 
    #endif

    // Gammar correction
    colour_out = vec3( sqrt(colour_out[0]), sqrt(colour_out[1]), sqrt(colour_out[2]) );
}

void render(hitable* world, int xs, int ys, render_callback_t callback, void* userdata) {
    const camera cam;

    #ifdef RAYT_OPENMP
    #pragma omp parallel for
    #endif
    for(int j = 0; j < ys; ++j) {
            #ifdef RAYT_OPENMP
            #pragma omp parallel for
            #endif
            for(int i = 0; i < xs; ++i) {
                vec3 colour(0.0, 0.0, 0.0);
                render_pixel(world, cam, xs, ys, i, j, colour);
                callback(userdata, i, j, colour);
            }
    }
}

#endif