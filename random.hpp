#ifndef _RANDOM_HPP_
#define _RANDOM_HPP_

#include "vec3.hpp"

extern double drand48(void);

float get_rand() {
    return (float)drand48();
}

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(),drand48(),drand48()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

/*vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(get_rand(), get_rand(), get_rand()) - vec3(1, 1, 1);
    } while(p.squared_length() >= 1.0);
    return p;
}*/

#endif
