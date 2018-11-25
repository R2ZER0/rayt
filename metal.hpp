#ifndef _METAL_HPP_
#define _METAL_HPP_

#include "material.hpp"
#include "random.hpp"

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

class metal : public material {
  public:
    metal(const vec3& a, float fuz) : albedo(a), fuzz(fuz) {
        if(fuzz > 1.0) { fuzz = 1.0; }
    }
    
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    vec3 albedo;
    float fuzz;
};

#endif
