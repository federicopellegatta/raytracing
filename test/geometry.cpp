#include "geometry.h"
#include <cassert>
#include <cstdlib>

using namespace fmt;

int main() {
  Vec a(1.0, 2.0, 3.0);
  Vec b(4.0, 6.0, 8.0);
  Point p(1.0, 2.0, 3.0);
  Point q(4.0, 6.0, 8.0);

  // print(q.to_str());

  // check compare operations
  assert(a == a);
  assert(a != b);
  assert(p == p);
  assert(p != q);

  // check Vec operations
  assert(a + b == Vec(5.0, 8.0, 11.0));
  assert(b - a == Vec(3.0, 4.0, 5.0));
  assert(a * 2 == Vec(2.0, 4.0, 6.0));
  assert(a.dot(b) == 40.0);
  assert(a.cross(b) == Vec(-2.0, 4.0, -2.0));
  assert(b.cross(a) == Vec(2.0, -4.0, 2.0));
  assert(are_close(a.squared_norm(), 14.0));
  assert(are_close(pow(a.norm(), 2), 14.0));

  // check Point operations
  assert(p * 2 == Point(2.0, 4.0, 6.0));
  assert(q + a == Point(5.0, 8.0, 11.0));
  assert(q - p == Vec(3.0, 4.0, 5.0));

  // check Matrix constructor
  float m1[4][4] = {{1.0, 2.0, 3.0, 4.0},
                    {5.0, 6.0, 7.0, 8.0},
                    {9.0, 9.0, 8.0, 7.0},
                    {6.0, 5.0, 4.0, 1.0}};
  float minv1[4][4] = {{-3.75, 2.75, -1.0, 0.0},
                       {4.375, -3.875, 2.0, -0.5},
                       {0.5, 0.5, -1.0, 1.0},
                       {-1.375, 0.875, 0.0, -0.5}};

  Transformation t1{m1, minv1};
  // assert(t1.is_consistent());

  // print(t1.to_str());

  return 0;
}