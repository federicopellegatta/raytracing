#include "shapes.h"
#include <cassert>

using namespace std;

void test_sphere_hit() {
  Sphere sphere;
  Ray ray1(Point(0., 0., 1.), -1 * VEC_Z);

  HitRecord intersection1 = sphere.ray_intersection(ray1);
  assert(intersection1.hit);
  assert(HitRecord(Point(0., 0., 1.), Normal(0., 0., 1.), Vec2d(0., 0.), 1.0,
                   ray1, false)
             .is_close(intersection1));

  Ray ray2 = Ray(Point(3, 0, 0), -1 * VEC_X);
  HitRecord intersection2 = sphere.ray_intersection(ray2);
  assert(intersection2.hit);
  assert(HitRecord(Point(1.0, 0.0, 0.0), Normal(1.0, 0.0, 0.0), Vec2d(0.0, 0.5),
                   2.0, ray2, false)
             .is_close(intersection2));

  assert(!sphere.ray_intersection(Ray(Point(0, 10, 2), -1 * VEC_Z)).hit);
}

void test_inner_hit() {
  Sphere sphere;

  Ray ray(Point(0, 0, 0), VEC_X);
  HitRecord intersection = sphere.ray_intersection(ray);
  assert(intersection.hit);
  assert(HitRecord(Point(1.0, 0.0, 0.0), Normal(-1.0, 0.0, 0.0),
                   Vec2d(0.0, 0.5), 1.0, ray, false)
             .is_close(intersection));
}

void test_sphere_transformation() {
  Sphere sphere{translation(Vec(10.0, 0.0, 0.0))};

  Ray ray1{Point(10, 0, 2), -1 * VEC_Z};
  HitRecord intersection1 = sphere.ray_intersection(ray1);
  assert(intersection1.hit);
  assert(HitRecord(Point(10.0, 0.0, 1.0), Normal(0.0, 0.0, 1.0),
                   Vec2d(0.0, 0.0), 1.0, ray1, false)
             .is_close(intersection1));

  Ray ray2{Point(13, 0, 0), -1 * VEC_X};
  HitRecord intersection2 = sphere.ray_intersection(ray2);
  assert(intersection2.hit);
  assert(HitRecord(Point(11.0, 0.0, 0.0), Normal(1.0, 0.0, 0.0),
                   Vec2d(0.0, 0.5), 2.0, ray2, false)
             .is_close(intersection2));

  // Check if the sphere failed to move by trying to hit the untransformed shape
  assert(!sphere.ray_intersection(Ray(Point(0, 0, 2), -1 * VEC_Z)).hit);

  // Check if the *inverse *transformation was wrongly applied
  assert(!sphere.ray_intersection(Ray(Point(-10, 0, 0), -1 * VEC_Z)).hit);
}

void test_sphere_normal() {
  Sphere sphere{scaling(Vec(2.0, 1.0, 1.0))};

  Ray ray{Point(1.0, 1.0, 0.0), Vec(-1.0, -1.0)};
  HitRecord intersection = sphere.ray_intersection(ray);

  // We normalize "intersection.normal", as we are not interested in its length
  assert(intersection.normal.normalize() == Normal(1.0, 4.0, 0.0).normalize());
}

int main() {
  test_sphere_hit();
  return 0;
}