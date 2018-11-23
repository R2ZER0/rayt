#include <math.h>
#include <stdlib.h>
#include <iostream>

struct vec3 {
  vec3() {}
  vec3(float v0, float v1, float v2) {
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
  }

  inline float x() const { return v[0]; }
  inline float y() const { return v[1]; }
  inline float z() const { return v[2]; }

  inline float r() const { return v[0]; }
  inline float g() const { return v[1]; }
  inline float b() const { return v[2]; }

  inline const vec3& operator+() const { return *this; }
  inline vec3 operator-() const { return vec3(-v[0], -v[1], -v[3]); }
  inline float operator[](int i) const { return v[i]; }
  inline float& operator[](int i) { return v[i]; }

  inline vec3& operator+=(const vec3& v2);
  inline vec3& operator-=(const vec3& v2);
  inline vec3& operator*=(const vec3& v2);
  inline vec3& operator/=(const vec3& v2);
  inline vec3& operator*=(const float t);
  inline vec3& operator/=(const float t);

  inline float length() const {
    return sqrt(squared_length());
  }

  inline float squared_length() const {
    return x()*x() + y()*y() + z()*z();
  }

  inline void make_unit_vector();

  float v[3];
};

inline std::istream& operator>>(std::istream& is, vec3 &t) {
  is >> t.v[0] >> t.v[1] >> t.v[2];
  return is;
}

inline std::ostream& operator<<(std::ostream& os, vec3& t) {
  os << t.v[0] << ' ' << t.v[1] << ' ' << t.v[2];
  return os;
}

inline void vec3::make_unit_vector() {
  float k = 1.0 / length();
  v[0] *= k;
  v[1] *= k;
  v[2] *= k;
}

inline vec3 operator+(const vec3& v1, const vec3& v2) {
  return vec3(v1[0]+v2[0], v1[1]+v2[1], v1[2]+v2[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2) {
  return vec3(v1[0]-v2[0], v1[1]-v2[1], v1[2]-v2[2]);
}

inline vec3 operator*(const vec3& v1, const vec3& v2) {
  return vec3(v1[0]*v2[0], v1[1]*v2[1], v1[2]*v2[2]);
}

inline vec3 operator/(const vec3& v1, const vec3& v2) {
  return vec3(v1[0]/v2[0], v1[1]/v2[1], v1[2]/v2[2]);
}

inline vec3 operator*(const vec3& v, float t) {
  return vec3(v[0]*t, v[1]*t, v[2]*t);
}

inline vec3 operator/(const vec3& v, float t) {
  return vec3(v[0]/t, v[1]/t, v[2]/t);
}

inline float dot(const vec3& v1, const vec3& v2) {
  return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
  return vec3( v1[1] * v2[2] - v1[2] * v2[1],
               -(v1[0] * v2[2] - v1[2] * v2[0]),
               v1[0] * v2[1] - v1[1] * v2[0]
              );
}

inline vec3& vec3::operator+=(const vec3& vo) {
  v[0] += vo[0];
  v[1] += vo[1];
  v[2] += vo[2];
  return *this;
}

inline vec3& vec3::operator-=(const vec3& vo) {
  v[0] -= vo[0];
  v[1] -= vo[1];
  v[2] -= vo[2];
  return *this;
}

inline vec3& vec3::operator*=(const vec3& vo) {
  v[0] *= vo[0];
  v[1] *= vo[1];
  v[2] *= vo[2];
  return *this;
}

inline vec3& vec3::operator/=(const vec3& vo) {
  v[0] /= vo[0];
  v[1] /= vo[1];
  v[2] /= vo[2];
  return *this;
}

inline vec3& vec3::operator*=(const float t) {
  v[0] *= t;
  v[1] *= t;
  v[2] *= t;
  return *this;
}

inline vec3& vec3::operator/=(const float t) {
  v[0] /= t;
  v[1] /= t;
  v[2] /= t;
  return *this;
}

inline vec3 unit_vector(vec3 v) {
  return v / v.length();
}
