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

  Normal operator-() const { return Normal{-x, -y, -z}; }
};

extern Vec VEC_X;
extern Vec VEC_Y;
extern Vec VEC_Z;

// Sum operation between Vecs and Points
Vec operator+(const Vec &, const Vec &);
Point operator+(const Point &, const Vec &);

// Mul operation between Vecs/Points and a floating point (and viceversa)
Vec operator*(const float &, const Vec &);
Vec operator*(const Vec &, const float &);

Normal operator*(const float &, const Normal &);

Point operator*(const float &, const Point &);
Point operator*(const Point &, const float &);

// Minus operation between Vecs and Points
Vec operator-(const Vec &, const Vec &);
Normal operator-(const Normal &, const Normal &);
Vec operator-(const Point &, const Point &);
Point operator-(const Point &, const Vec &);

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
