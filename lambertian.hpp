#ifndef _LAMBERTIAN_HPP_
#define _LAMBERTIAN_HPP_

#include "material.hpp"

#ifndef RAYT_DETERMINISTIC
#include "random.hpp"
#endif

class lambertian : public material {
  public:
    lambertian(const vec3& a) : albedo(a) {}
    
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        #ifdef RAYT_DETERMINISTIC
        vec3 target = rec.p + rec.normal;
        #else
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        #endif
        scattered = ray(rec.p, target-rec.p);
        attenuation = albedo;
        return true;
    }

    vec3 albedo;
};

#endif
