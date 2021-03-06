#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

class camera {
  public:
    camera() {
        lower_left_corner = vec3(-2.1, -1.0, -1.0);
        horizontal = vec3(4.0, 0.0, 0.0);
        vertical = vec3(0.0, 2.0, 0.0);
        origin = vec3(0.0, 0.0, 0.5);
    }

    ray get_ray(float u, float v) const {
        return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
    }

    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 origin;
};
    

#endif
