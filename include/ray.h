#include "geometry.h"
#include <iostream>
#include <limits>

using namespace std;

struct Ray {
  Point origin = Point();
  Vec dir = Vec();
  float tmin = 1e-5;
  float tmax = numeric_limits<float>::infinity();
  int depth = 0;

  Ray();

  bool is_close(Ray other) {
    return (origin == other.origin) && (dir == other.dir);
  }
};