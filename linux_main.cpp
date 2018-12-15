#ifdef RAYT_LINUX

#include "render.hpp"

struct imagedata {
        int xs, ys;
        vec3* colourdata;
};

void _render_callback_to_imagedata(void* userdata, int x, int y, vec3 colour) {
    imagedata* img = (imagedata*)userdata;
    int idx = (img->ys-y-1)*img->xs + x;
    if(idx > img->xs*img->ys || idx < 0) {
        #ifdef RAYT_LINUX
        std::cerr << "Index out of range! " << idx << "i,j = " << x << ',' << y << '\n';
        #endif
    } else {
        img->colourdata[idx] = colour;

    }
}

void render_to_imagedata(hitable* world, imagedata& img) {
    render(world, img.xs, img.ys, _render_callback_to_imagedata, (void*)&img);
}

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

int main() {
    render_to_imagedata(&world, image);
    print_imagedata(image);
}

#endif