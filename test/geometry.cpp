#include "geometry.h"
#include <cassert>
#include <cstdlib>

using namespace std;

//////////////////////////////
/* TRANSFORMATION UNIT TESTS*/
//////////////////////////////

void test_transformation_constructor(float m1[4][4], float minv1[4][4]) {
  Transformation t1{m1, minv1};
  assert(t1.is_consistent());
  // fmt::print(t1.to_str());
  Transformation t2{m1, minv1};
  assert(t1.is_close(t2));
  Transformation t3{m1, minv1};
  t3.m[2][2] += 1.0; // making t3 not consistent
  assert(!t1.is_close(t3));
  Transformation t4{m1, minv1};
  t4.m[2][2] += 1.0; // making t4 not consistent
  assert(!t1.is_close(t4));
}

void test_transformation_mul(float m1[4][4], float minv1[4][4]) {
  Transformation t1{m1, minv1};
}
int main() {

  /////////////////////////////////
  /* VECS, POINT AND NORMAL TESTS*/
  /////////////////////////////////

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

  /////////////////////////
  /* TRANSFORMATION TESTS*/
  /////////////////////////
  float m1[4][4] = {{1.0, 2.0, 3.0, 4.0},
                    {5.0, 6.0, 7.0, 8.0},
                    {9.0, 9.0, 8.0, 7.0},
                    {6.0, 5.0, 4.0, 1.0}};
  float minv1[4][4] = {{-3.75, 2.75, -1.0, 0.0},
                       {4.375, -3.875, 2.0, -0.5},
                       {0.5, 0.5, -1.0, 1.0},
                       {-1.375, 0.875, 0.0, -0.5}};

  test_transformation_constructor(m1, minv1);

  return 0;
}