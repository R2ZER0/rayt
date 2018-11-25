#ifndef _DIELECTRIC_HPP_
#define _DIELECTRIC_HPP_

#include "material.hpp"
#include "random.hpp"

// works
bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if(discriminant > 0) {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}


// works
class dielectric : public material {
  public:
    dielectric(float ri) : albedo(vec3(0,0,0)), ref_idx(ri) {}
    
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;

        vec3 refracted;

        if(dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;

        } else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
        }

        attenuation = vec3(1.0, 1.0, 1.0);
        if(refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            scattered = ray(rec.p, refracted);
        } else {
            scattered = ray(rec.p, reflected);
            return false;
        }
        return true;
    }

    vec3 albedo;
    float ref_idx;
};



#endif
