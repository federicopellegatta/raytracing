#include "colors.h"
#include <iostream>

using namespace std;

template <typename In> string convert_to_string(const In &a) {
  return string{"(" + to_string(a.x) + ", " + to_string(a.y) + ", " +
                to_string(a.z) + ")"};
}

template <typename In> bool are_close(const In &a, const In &b) {
  return are_close(a.x, b.x) && are_close(a.y, b.y) && are_close(a.z, b.z);
}

struct Vec {
  float x, y, z;

  Vec(float _x = 0, float _y = 0, float _z = 0) : x{_x}, y{_y}, z{_z} {}
  Vec(const Vec &);  // Copy constructor
  Vec(const Vec &&); // Move constructor
  inline string to_str() { return string{"Vec" + convert_to_string(*this)}; }
  inline bool is_close(Vec a) { return are_close<Vec>(*this, a); }
};

struct Point {
  float x, y, z;

  Point(float _x = 0, float _y = 0, float _z = 0) : x{_x}, y{_y}, z{_z} {}
  Point(const Point &);  // Copy constructor
  Point(const Point &&); // Move constructor
  inline string to_str() { return string{"Point" + convert_to_string(*this)}; }
  inline bool is_close(Point a) { return are_close<Point>(*this, a); }
};

template <typename In1, typename In2, typename Out>
Out _sum(const In1 &a, const In2 &b) {
  return Out{a.x + b.x, a.y + b.y, a.z + b.z};
}

Point operator+(const Point &a, const Vec &b) {
  return _sum<Point, Vec, Point>(a, b);
}

Vec operator+(const Vec &a, const Vec &b) { return _sum<Vec, Vec, Vec>(a, b); }
