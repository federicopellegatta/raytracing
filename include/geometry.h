#include "colors.h"
#include <cmath>
#include <iostream>
#include <valarray>

using namespace std;

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

struct Vec {
  float x, y, z;

  Vec(float _x = 0, float _y = 0, float _z = 0) : x{_x}, y{_y}, z{_z} {}
  Vec(const Vec &);  // Copy constructor
  Vec(const Vec &&); // Move constructor

  // Scalar and Vector product
  inline float dot(const Vec &a) { return a.x * x + a.y * y + a.z * z; }
  inline Vec cross(const Vec &b) {
    return Vec(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
  }

  inline float squared_norm() { return pow(x, 2) + pow(y, 2) + pow(z, 2); }
  inline float norm() { return sqrt(squared_norm()); }
  inline void normalize() {
    float norm = this->norm();
    x /= norm;
    y /= norm;
    z /= norm;
  }
  inline string to_str() { return string{"Vec" + _to_string(*this)}; }
};

struct Point {
  float x, y, z;

  Point(float _x = 0, float _y = 0, float _z = 0) : x{_x}, y{_y}, z{_z} {}
  Point(const Point &);  // Copy constructor
  Point(const Point &&); // Move constructor
  inline string to_str() { return string{"Point" + _to_string(*this)}; }
};

struct Normal {
  float x, y, z;
};

// Define a generic transformation
// from two 4x4 matrices
struct Transformation {
  float m[4][4];
  float invm[4][4];

  Transformation();
  Transformation(float[4][4], float[4][4]);
  Vec operator*(Vec);
  Point operator*(Point);
  Normal operator*(Normal);
  Transformation operator*(Transformation);
};

// Sum operation between Vecs and Points
Vec operator+(const Vec &a, const Vec &b) { return _sum<Vec, Vec, Vec>(a, b); }
Point operator+(const Point &a, const Vec &b) {
  return _sum<Point, Vec, Point>(a, b);
}

// Mul operation between Vecs/Points and a floating point (and viceversa)
Vec operator*(const float &a, const Vec &b) { return _mul<Vec, Vec>(a, b); }
Vec operator*(const Vec &a, const float &b) { return b * a; }

Point operator*(const float &a, const Point &b) {
  return _mul<Point, Point>(a, b);
}
Point operator*(const Point &a, const float &b) { return b * a; }

// Minus operation between Vecs and Points
Vec operator-(const Vec &a, const Vec &b) {
  return _sum<Vec, Vec, Vec>(a, -1 * b);
}
Vec operator-(const Point &a, const Point &b) {
  return _sum<Point, Point, Vec>(a, -1 * b);
}
Point operator-(const Point &a, const Vec &b) {
  return _sum<Point, Vec, Point>(a, -1 * b);
}

// Trasformazioni
Transformation translation(Vec);

// Matrix operation
void _matr_prod(const float[4][4], const float[4][4], float[4][4]);