#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include "vec3.hpp"
#include "ray.hpp"

class material {
  public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

#endif
