#include <cmath>
#include <iostream>

using namespace std;

struct Vec {
  float x, y, z;

  Vec(float _x = 0, float _y = 0, float _z = 0) : x{_x}, y{_y}, z{_z} {}
  Vec(const Vec &);  // Copy constructor
  Vec(const Vec &&); // Move constructor

  inline float squared_norm() { return pow(x, 2) + pow(y, 2) + pow(z, 2); }
  inline float norm() { return sqrt(this->squared_norm()); }
  inline void normalize() {
    float norm = this->norm();
    x /= norm;
    y /= norm;
    z /= norm;
  }
};
struct Point {
  float x, y, z;

  Point(float _x = 0, float _y = 0, float _z = 0) : x{_x}, y{_y}, z{_z} {}
  Point(const Point &);  // Copy constructor
  Point(const Point &&); // Move constructor
};

// Template function for sum
// Summing points and/or vectors is the same thing in 3D
template <typename In1, typename In2, typename Out>
Out _sum(const In1 &a, const In2 &b) {
  return Out{a.x + b.x, a.y + b.y, a.z + b.z};
}
template <typename In1, typename Out> Out _mul(const float &a, const In1 &b) {
  return Out{a * b.x, a * b.y, a * b.z};
}

// Definition of + between points
Point operator+(const Point &a, const Vec &b) {
  return _sum<Point, Vec, Point>(a, b);
}

// Definitions and overloads of basic operations between vectors
Vec operator+(const Vec &a, const Vec &b) { return _sum<Vec, Vec, Vec>(a, b); }
inline Vec operator*(const float &a, const Vec &b) {
  return _mul<Vec, Vec>(a, b);
}
inline Vec operator*(const Vec &a, const float &b) { return b * a; }
Vec operator-(const Vec &a, const Vec &b) {
  return _sum<Vec, Vec, Vec>(a, -1 * b);
}
Point operator*(const float &a, const Point &b) {
  return _mul<Point, Point>(a, b);
}
Vec operator-(const Point &a, const Point &b) {
  return _sum<Point, Point, Vec>(a, -1 * b);
}
Point operator-(const Point &a, const Vec &b) {
  return _sum<Point, Vec, Point>(a, -1 * b);
}
inline float dot(const Vec &a, const Vec &b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline Vec cross(const Vec &a, const Vec &b) {
  return Vec(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
             a.x * b.y - a.y * b.x);
}
