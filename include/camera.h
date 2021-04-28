#include "geometry.h"
#include <iostream>
#include <limits>

using namespace std;

struct ray {
  Point origin;
  Vec direction;
  float tmin = 1e-5;
  float tmax = numeric_limits<float>::infinity();
  int depth = 0;
};