#include "shapes.h"
#include <cassert>

using namespace std;

void test_sphere_hit() {
  Sphere sphere;
  Ray ray1(Point(0., 0., 2.), -1 * VEC_Z);

  HitRecord intersection1 = sphere.ray_intersection(ray1);
  assert(intersection1.hit);
  assert(HitRecord(Point(0., 0., 1.), Normal(0., 0., 1.), Vec2d(0., 0.), 1.0,
                   ray1, false)
             .is_close(intersection1));
}
int main() {
  test_sphere_hit();
  return 0;
}