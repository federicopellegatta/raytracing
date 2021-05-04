#include "camera.h"
#include <cassert>

using namespace std;

void test_orthogonal_camera() {
  OrthogonalCamera cam(2.0, Transformation());

  Ray ray1 = cam.fire_ray(0.0, 0.0);
  Ray ray2 = cam.fire_ray(1.0, 0.0);
  Ray ray3 = cam.fire_ray(0.0, 1.0);
  Ray ray4 = cam.fire_ray(1.0, 1.0);

  // Verify that the rays are parallel by verifying that cross-products vanish
  assert(are_close(0.0, ray1.dir.cross(ray2.dir).squared_norm()));
  assert(are_close(0.0, ray1.dir.cross(ray3.dir).squared_norm()));
  assert(are_close(0.0, ray1.dir.cross(ray4.dir).squared_norm()));

  // Verify that the ray hitting the corners have the right coordinates
  assert(ray1.at(1.0) == Point(0.0, 2.0, -1.0));
  assert(ray2.at(1.0) == Point(0.0, -2.0, -1.0));
  assert(ray3.at(1.0) == Point(0.0, 2.0, 1.0));
  assert(ray4.at(1.0) == Point(0.0, -2.0, 1.0));
}

void test_orthogonal_camera_transform() {
  float _aspect_ratio;
  Transformation t = translation(VEC_Y * -2.0) * rotation_z(M_PI / 2.0);
  OrthogonalCamera cam(_aspect_ratio, t);

  Ray ray = cam.fire_ray(0.5, 0.5);
  assert(ray.at(1.0) == Point(0.0, -2.0, 0.0));
}

void test_perspective_camera() {
  PerspectiveCamera cam(1.0, 2.0, Transformation());

  // Fire one ray for each corner of the image plane
  Ray ray1 = cam.fire_ray(0.0, 0.0);
  Ray ray2 = cam.fire_ray(1.0, 0.0);
  Ray ray3 = cam.fire_ray(0.0, 1.0);
  Ray ray4 = cam.fire_ray(1.0, 1.0);

  // Verify that all the rays depart from the same point
  assert(ray1.origin == ray2.origin);
  assert(ray1.origin == ray3.origin);
  assert(ray1.origin == ray4.origin);

  // Verify that the ray hitting the corners have the right coordinates
  assert(ray1.at(1.0) == Point(0.0, 2.0, -1.0));
  assert(ray2.at(1.0) == Point(0.0, -2.0, -1.0));
  assert(ray3.at(1.0) == Point(0.0, 2.0, 1.0));
  assert(ray4.at(1.0) == Point(0.0, -2.0, 1.0));
}

void test_perspective_camera_transform() {
  float _screen_distance, _aspect_ratio;
  Transformation t = translation(VEC_Y * -2.0) * rotation_z(M_PI / 2.0);
  PerspectiveCamera cam(_screen_distance, _aspect_ratio, t);

  Ray ray = cam.fire_ray(0.5, 0.5);
  assert(ray.at(1.0) == Point(0.0, -2.0, 0.0));
}

int main() {
  test_orthogonal_camera();
  test_orthogonal_camera_transform();
  test_perspective_camera();
  test_perspective_camera_transform();

  return 0;
}