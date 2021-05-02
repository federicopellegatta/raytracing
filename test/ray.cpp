#include "ray.h"
#include <cassert>

using namespace std;

void test_is_close() {
  Ray ray1(Point(1.0, 2.0, 3.0), Vec(5.0, 4.0, -1.0));
  Ray ray2(Point(1.0, 2.0, 3.0), Vec(5.0, 4.0, -1.0));
  Ray ray3(Point(5.0, 1.0, 4.0), Vec(3.0, 9.0, 4.0));

  assert(ray1 == ray2);
  assert(!(ray1 == ray3));
}

void test_at() {
  Ray ray(Point(1.0, 2.0, 4.0), Vec(4.0, 2.0, 1.0));

  assert(ray.at(0.0) == ray.origin);
  assert(ray.at(1.0) == Point(5.0, 4.0, 5.0));
  assert(ray.at(2.0) == Point(9.0, 6.0, 6.0));
}

void test_transformation() {
  Ray ray(Point(1.0, 2.0, 3.0), Vec(6.0, 5.0, 4.0));
  Transformation transformation =
      translation(Vec(10.0, 11.0, 12.0)) * rotation_x(M_PI / 2.0);
  Ray transformed = ray.transform(transformation);

  assert(transformed.origin == Point(11.0, 8.0, 14.0));
  assert(transformed.dir == Vec(6.0, -4.0, 5.0));
}

int main() {
  test_is_close();
  test_at();
  test_transformation();
}