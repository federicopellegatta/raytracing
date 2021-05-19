#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "colors.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

////////////////////////////
/* VECS, POINT AND NORMAL */
////////////////////////////

// Template functions
template <typename In> string _to_string(const In &a) {
  return string{"(" + to_string(a.x) + ", " + to_string(a.y) + ", " +
                to_string(a.z) + ")"};
}

template <typename In> bool _are_close(const In &a, const In &b) {
  return are_close(a.x, b.x) && are_close(a.y, b.y) && are_close(a.z, b.z);
}

// Wrapper around _are_close. More elegant ;)
template <typename In> bool operator==(const In &a, const In &b) {
  return _are_close<In>(a, b);
}
template <typename In> bool operator!=(const In &a, const In &b) {
  return !(_are_close<In>(a, b));
}

template <typename In1, typename In2, typename Out>
Out _sum(const In1 &a, const In2 &b) {
  return Out{a.x + b.x, a.y + b.y, a.z + b.z};
}

template <typename In1, typename Out> Out _mul(const float &a, const In1 &b) {
  return Out{a * b.x, a * b.y, a * b.z};
}

/** Vec class
 * @brief A class which represent a 3D vector
 */
struct Vec {
  float x, y, z;

  Vec(float _x = 0, float _y = 0, float _z = 0) : x{_x}, y{_y}, z{_z} {}

  // Scalar and Vector product
  inline float dot(const Vec &a) { return a.x * x + a.y * y + a.z * z; }
  inline Vec cross(const Vec &b) {
    return Vec(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
  }

  inline float squared_norm() { return pow(x, 2) + pow(y, 2) + pow(z, 2); }
  inline float norm() { return sqrt(squared_norm()); }
  inline Vec normalize() {
    float norm = this->norm();
    return Vec{x /= norm, y /= norm, z /= norm};
  }
  inline string to_str() { return string{"Vec" + _to_string(*this)}; }

  Vec operator-() const { return Vec{-x, -y, -z}; }
};

/** Point class
 * @brief A class which represent a 3D point
 */
struct Point {
  float x, y, z;

  Point(float _x = 0, float _y = 0, float _z = 0) : x{_x}, y{_y}, z{_z} {}
  inline string to_str() { return string{"Point" + _to_string(*this)}; }
  inline Vec to_vec() { return Vec(x, y, z); }
};

/** Normal class
 * @brief A class which represent a 3D normal to a surface
 */
struct Normal {
  float x, y, z;

  Normal(float _x = 0, float _y = 0, float _z = 0) : x{_x}, y{_y}, z{_z} {}
  inline string to_str() { return string{"Normal" + _to_string(*this)}; }

  inline float squared_norm() { return x * x + y * y + z * z; }

  inline float norm() { return sqrt(squared_norm()); }
  inline Normal normalize() {
    float norm = this->norm();
    return Normal{x /= norm, y /= norm, z /= norm};
  }
  inline Vec to_vec() { return Vec(x, y, z); }
  Normal operator-() const { return Normal{-x, -y, -z}; }
};

extern Vec VEC_X;
extern Vec VEC_Y;
extern Vec VEC_Z;

// Sum operation between Vecs and Points
inline Vec operator+(const Vec &a, const Vec &b) {
  return _sum<Vec, Vec, Vec>(a, b);
}
inline Point operator+(const Point &a, const Vec &b) {
  return _sum<Point, Vec, Point>(a, b);
}

// Mul operation between Vecs/Points and a floating point (and viceversa)
inline Vec operator*(const float &a, const Vec &b) {
  return _mul<Vec, Vec>(a, b);
}
inline Vec operator*(const Vec &a, const float &b) { return b * a; }

inline Normal operator*(const float &a, const Normal &b) {
  return _mul<Normal, Normal>(a, b);
}

inline Point operator*(const float &a, const Point &b) {
  return _mul<Point, Point>(a, b);
}
inline Point operator*(const Point &a, const float &b) { return b * a; }

// Minus operation between Vecs and Points
inline Vec operator-(const Vec &a, const Vec &b) {
  return _sum<Vec, Vec, Vec>(a, -1 * b);
}
inline Normal operator-(const Normal &a, const Normal &b) {
  return _sum<Normal, Normal, Normal>(a, -1 * b);
}

inline Vec operator-(const Point &a, const Point &b) {
  return _sum<Point, Point, Vec>(a, -1 * b);
}
inline Point operator-(const Point &a, const Vec &b) {
  return _sum<Point, Vec, Point>(a, -1 * b);
}

////////////////////
/*   2D VECTOR    */
////////////////////

/**
 * @brief A 2D vector used to represent a point on a surface
 *
 */
struct Vec2d {
  /**
   * @brief The class members are named u and v to distinguish them from x,y,z,
   * which are used for 3D vectors
   *
   */
  float u, v;
  /**
   * @brief Construct a new Vec 2d object
   *
   * @param _u
   * @param _v
   */
  Vec2d(float _u = 0.f, float _v = 0.f) : u{_u}, v{_v} {}

  /**
   * @brief Check whether two 2d vectors are the same
   *
   * @param other_vec2d
   * @return true
   * @return false
   */
  inline bool is_close(Vec2d other_vec2d) {
    return are_close(u, other_vec2d.u) && are_close(v, other_vec2d.v);
  }
};

////////////////////
/* TRANSFORMATION */
////////////////////

extern float IDENTITY_MATR4x4[4][4];

/** Transformation class
 * @brief Define a generic transformation from two 4x4 matrices
 *
 * @param m A 4x4 matrix
 * @param invm `m` inverse matrix
 */
struct Transformation {
  float m[4][4] = {{1.f, 0.f, 0.f, 0.f},
                   {0.f, 1.f, 0.f, 0.f},
                   {0.f, 0.f, 1.f, 0.f},
                   {0.f, 0.f, 0.f, 1.f}};

  float invm[4][4] = {{1.f, 0.f, 0.f, 0.f},
                      {0.f, 1.f, 0.f, 0.f},
                      {0.f, 0.f, 1.f, 0.f},
                      {0.f, 0.f, 0.f, 1.f}};

  Transformation(){};
  Transformation(float[4][4], float[4][4]);

  /**
   * @brief Check if `m` * `invm` is the identity matrix
   */
  bool is_consistent();
  bool is_close(Transformation);
  string to_str();

  Transformation inverse();
  Vec operator*(Vec);
  Point operator*(Point);
  Normal operator*(Normal);
  Transformation operator*(Transformation);
};

// Comparison between 4x4 matrix
bool _are_matr_close(float[4][4], float[4][4]);
void _matr_prod(const float[4][4], const float[4][4], float[4][4]);

Transformation translation(Vec);
Transformation scaling(Vec);
Transformation rotation_x(float);
Transformation rotation_y(float);
Transformation rotation_z(float);

#endif
