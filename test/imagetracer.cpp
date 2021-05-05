#include "imagetracer.h"
#include <cassert>

using namespace std;

void test_uv_sub_mapping(ImageTracer tracer) {
  Ray ray1 = tracer.fire_ray(0, 0, 2.5, 1.5);
  Ray ray2 = tracer.fire_ray(2, 1, 0.5, 0.5);
  assert(ray1 == ray2);
}

void test_orientation(ImageTracer tracer) {
  Ray top_left_ray = tracer.fire_ray(0, 0, 0.0, 0.0);
  assert(Point(0.0, 2.0, 1.0) == top_left_ray.at(1.0));

  // Fire a ray against bottom - right corner of the screen
  Ray bottom_right_ray = tracer.fire_ray(3, 1, 1.0, 1.0);
  assert(Point(0.0, -2.0, -1.0) == bottom_right_ray.at(1.0));
}

void test_image_coverage(ImageTracer tracer) {
  tracer.fire_all_rays([](Ray ray) -> Color { return Color(1.0, 2.0, 3.0); });
  for (int row{}; row < tracer.image.height; row++) {
    for (int col{}; col < tracer.image.width; col++) {
      assert(tracer.image.get_pixel(col, row).is_close(Color(1.0, 2.0, 3.0)));
    }
  }
}
int main() {

  HdrImage img(4, 2);
  shared_ptr<Camera> camera =
      make_shared<PerspectiveCamera>(1.0, 2.0, Transformation());
  ImageTracer tracer(img, camera);

  test_uv_sub_mapping(tracer);
  test_image_coverage(tracer);
  test_orientation(tracer);

  return 0;
}