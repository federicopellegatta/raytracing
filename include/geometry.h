#include <iostream>

using namespace std;

struct Vec {
  float x, y, z;

  Vec(float _x = 0, float _y = 0, float _z = 0) : x{_x}, y{_y}, z{_z} {}
  Vec(const Vec &);  // Copy constructor
  Vec(const Vec &&); // Move constructor
};

struct Point {
  float x, y, z;

  Point(float _x = 0, float _y = 0, float _z = 0) : x{_x}, y{_y}, z{_z} {}
  Point(const Point &);  // Copy constructor
  Point(const Point &&); // Move constructor
};