#ifndef _RAY_HPP_
#define _RAY_HPP_

#include "vec3.hpp"

struct ray {
        vec3 A;
        vec3 B;

        ray() {}
        ray(const vec3& a, const vec3& b) {
                A = a;
                B = b;
        }
        inline vec3 origin() const { return A; }
        inline vec3 direction() const { return B; }
        inline vec3 point_at_parameter(float t) const { return A + t*B; }
};

#endif /* _RAY_HPP_ */
