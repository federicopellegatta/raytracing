#include "geometry.h"
#include <cassert>
#include <cstdlib>

using namespace std;

//////////////////////////////
/* TRANSFORMATION UNIT TESTS*/
//////////////////////////////

void test_transformation_constructor(float m1[4][4], float invm1[4][4]) {
  Transformation t1{m1, invm1};
  assert(t1.is_consistent());
  // fmt::print(t1.to_str());
  Transformation t2{m1, invm1};
  assert(t1.is_close(t2));
  Transformation t3{m1, invm1};
  t3.m[2][2] += 1.0; // making t3 not consistent
  assert(!t1.is_close(t3));
  Transformation t4{m1, invm1};
  t4.m[2][2] += 1.0; // making t4 not consistent
  assert(!t1.is_close(t4));
}

void test_transformation_mul(float m1[4][4], float invm1[4][4]) {
  Transformation t1{m1, invm1};
  assert(t1.is_consistent());
  float m2[4][4] = {{3.0, 5.0, 2.0, 4.0},
                    {4.0, 1.0, 0.0, 5.0},
                    {6.0, 3.0, 2.0, 0.0},
                    {1.0, 4.0, 2.0, 1.0}};
  float invm2[4][4] = {{0.4, -0.2, 0.2, -0.6},
                       {2.9, -1.7, 0.2, -3.1},
                       {-5.55, 3.15, -0.4, 6.45},
                       {-0.9, 0.7, -0.2, 1.1}};
  Transformation t2{m2, invm2};
  assert(t2.is_consistent());

  float m_expected[4][4] = {{33.0, 32.0, 16.0, 18.0},
                            {89.0, 84.0, 40.0, 58.0},
                            {118.0, 106.0, 48.0, 88.0},
                            {63.0, 51.0, 22.0, 50.0}};
  float invm_expected[4][4] = {{-1.45, 1.45, -1.0, 0.6},
                               {-13.95, 11.95, -6.5, 2.6},
                               {25.525, -22.025, 12.25, -5.2},
                               {4.825, -4.325, 2.5, -1.1}};
  Transformation t_expected{m_expected, invm_expected};

  assert(t_expected.is_consistent());
  assert(t_expected.is_close(t1 * t2));
}

void test_transformation_vec_point_mul() {
  float m[4][4] = {{1.0, 2.0, 3.0, 4.0},
                   {5.0, 6.0, 7.0, 8.0},
                   {9.0, 9.0, 8.0, 7.0},
                   {0.0, 0.0, 0.0, 1.0}};
  float invm[4][4] = {{-3.75, 2.75, -1, 0},
                      {5.75, -4.75, 2.0, 1.0},
                      {-2.25, 2.25, -1.0, -2.0},
                      {0.0, 0.0, 0.0, 1.0}};

  Transformation t{m, invm};
  assert(t.is_consistent());

  Vec v_expected(14.0, 38.0, 51.0);
  assert(v_expected == (t * Vec(1.0, 2.0, 3.0)));

  Point p_expected(18.0, 46.0, 58.0);
  assert(p_expected == (t * Point(1.0, 2.0, 3.0)));

  Normal n_expected(-8.75, 7.75, -3.0);
  assert(n_expected == (t * Normal(3.0, 2.0, 4.0)));
}

void test_transformation_inverse(float m1[4][4], float invm1[4][4]) {
  Transformation t1{m1, invm1};
  assert(t1.is_consistent());

  Transformation t2 = t1.inverse();
  assert(t2.is_consistent());

  Transformation prod = t1 * t2;
  assert(prod.is_consistent());
  assert(prod.is_close(Transformation()));
}

void test_transformation_translation() {
  Transformation tr1 = translation(Vec(1.0, 2.0, 3.0));
  assert(tr1.is_consistent());

  Transformation tr2 = translation(Vec(4.0, 6.0, 8.0));
  assert(tr2.is_consistent());

  Transformation prod = tr1 * tr2;
  assert(prod.is_consistent());

  Transformation expected = translation(Vec(5.0, 8.0, 11.0));
  assert(prod.is_close(expected));
}

void test_transformation_rotations() {
  assert(rotation_x(0.1).is_consistent());
  assert(rotation_y(0.1).is_consistent());
  assert(rotation_z(0.1).is_consistent());

  assert((rotation_x(M_PI / 2) * Vec(0., 1., 0.)) == Vec(0., 0., 1.));
  assert((rotation_y(M_PI / 2) * Vec(0., 0., 1.)) == Vec(1., 0., 0.));
  assert((rotation_z(M_PI / 2) * Vec(1., 0., 0.)) == Vec(0., 1., 0.));
}

void test_transformation_scaling() {
  Transformation tr1 = scaling(Vec(2.0, 5.0, 10.0));
  assert(tr1.is_consistent());

  Transformation tr2 = scaling(Vec(3.0, 2.0, 4.0));
  assert(tr2.is_consistent());

  Transformation expected = scaling(Vec(6.0, 10.0, 40.0));
  assert(expected.is_close(tr1 * tr2));
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
  float invm1[4][4] = {{-3.75, 2.75, -1.0, 0.0},
                       {4.375, -3.875, 2.0, -0.5},
                       {0.5, 0.5, -1.0, 1.0},
                       {-1.375, 0.875, 0.0, -0.2}}; // 0.5

  test_transformation_constructor(m1, invm1);
  test_transformation_mul(m1, invm1);
  test_transformation_vec_point_mul();
  test_transformation_inverse(m1, invm1);
  test_transformation_translation();
  test_transformation_rotations();
  test_transformation_scaling();

  return 0;
}