#define RAYT_ARDUINO

#include "render.hpp"

void _render_callback_to_serial(void* userdata, int x, int y, vec3 colour) {
    Serial.write(x):
    Serial.write('\t');
    Serial.write(y);
    Serial.write('\t');
    Serial.write(colour[0]);
    Serial.write('\t');
    Serial.write(colour[1]);
    Serial.write('\t');
    Serial.writeln(colour[2]);
}

void render_to_serial(hitable* world, int xs, int ys) {
    render(world, xs, ys, _render_callback_to_serial, (void*)NULL);
}


// Global scene data
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

void setup() {
    Serial.begin(115200);
}


void loop() {
    render_to_serial(&world, 64, 64);
}