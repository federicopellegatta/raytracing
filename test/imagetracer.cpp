#include "imagetracer.h"
#include <cassert>

using namespace std;

Color set_color(Ray ray) { return Color(1.0, 2.0, 3.0); }
void test_imagetracer() {
  HdrImage img(4, 2);
  //  PerspectiveCamera cam(1.0, 2.0, Transformation());
  shared_ptr<Camera> camera =
      make_shared<PerspectiveCamera>(1.0, 2.0, Transformation());
  ImageTracer tracer(img, camera);

  Ray ray1 = tracer.fire_ray(0, 0, 2.5, 1.5);
  Ray ray2 = tracer.fire_ray(2, 1, 0.5, 0.5);
  assert(ray1 == ray2);

  /*
  Color lambda_func = [](Ray ray) -> Color { return Color(1.0, 2.0, 3.0); };
  Color test_lambda = lambda_func;
  fmt::print("Test if lambda is working \n r = {}, g = {}, b = {} \n",
             test_lambda.r, test_lambda.g, test_lambda.b);
  */

  tracer.fire_all_rays([](Ray ray) -> Color { return Color(1.0, 2.0, 3.0); });
  for (int row{}; row < img.height; row++) {
    for (int col{}; col < img.width; col++) {
      assert(img.get_pixel(col, row).is_close(Color(1.0, 2.0, 3.0)));
      /*
      Color img_color = img.get_pixel(col, row);
      fmt::print("Color: r = {}, g = {}, b = {} \n", img_color.r, img_color.g,
                 img_color.b);
      */
    }
  }
}
int main() {
  test_imagetracer();

  return 0;
}