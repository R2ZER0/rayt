#ifndef _HITABLE_HPP_
#define _HITABLE_HPP_

#include "vec3.hpp"
#include "ray.hpp"

class material;

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    material* mat;
};

class hitable {
  public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif
