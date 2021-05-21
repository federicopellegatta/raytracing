#include "render.h"
#include "HdrImage.h"
#include "camera.h"
#include "imagetracer.h"
#include <cassert>

using namespace std;

void test_OnOff_render() {

  Sphere sphere{
      translation(Vec(2.0, 0.0, 0.0)) * scaling(Vec(0.2, 0.2, 0.2)),
      Material(make_shared<DiffusiveBRDF>(make_shared<UniformPigment>(WHITE)))};
  HdrImage image(3, 3);
  OrthogonalCamera camera(1.);
  ImageTracer tracer(image, make_shared<OrthogonalCamera>(camera));
  World world;
  world.add(make_shared<Sphere>(sphere));
  OnOffRenderer renderer(world);
  tracer.fire_all_rays(renderer);

  assert(image.get_pixel(0, 0).is_close(BLACK));
  assert(image.get_pixel(1, 0).is_close(BLACK));
  assert(image.get_pixel(2, 0).is_close(BLACK));

  assert(image.get_pixel(0, 1).is_close(BLACK));
  cout << image.get_pixel(1, 1).r << " " << image.get_pixel(1, 1).g << " "
       << image.get_pixel(1, 1).b << " " << endl;
  assert(image.get_pixel(1, 1).is_close(WHITE));
  assert(image.get_pixel(2, 1).is_close(BLACK));

  assert(image.get_pixel(0, 2).is_close(BLACK));
  assert(image.get_pixel(1, 2).is_close(BLACK));
  assert(image.get_pixel(2, 2).is_close(BLACK));
}

void test_flat_render() {

  Color sphere_color(1.0, 2.0, 3.0);
  Sphere sphere{translation(Vec(2.0, 0.0, 0.0)) * scaling(Vec(0.2, 0.2, 0.2)),
                Material(make_shared<DiffusiveBRDF>(
                    make_shared<UniformPigment>(sphere_color)))};
  HdrImage image(3, 3);
  OrthogonalCamera camera(1.);
  ImageTracer tracer(image, make_shared<OrthogonalCamera>(camera));
  World world;
  world.add(make_shared<Sphere>(sphere));
  FlatRenderer renderer(world);
  tracer.fire_all_rays(renderer);

  assert(image.get_pixel(0, 0).is_close(BLACK));
  assert(image.get_pixel(1, 0).is_close(BLACK));
  assert(image.get_pixel(2, 0).is_close(BLACK));

  assert(image.get_pixel(0, 1).is_close(BLACK));
  cout << image.get_pixel(1, 1).r << " " << image.get_pixel(1, 1).g << " "
       << image.get_pixel(1, 1).b << " " << endl;
  assert(image.get_pixel(1, 1).is_close(sphere_color));
  assert(image.get_pixel(2, 1).is_close(BLACK));

  assert(image.get_pixel(0, 2).is_close(BLACK));
  assert(image.get_pixel(1, 2).is_close(BLACK));
  assert(image.get_pixel(2, 2).is_close(BLACK));
}

int main() {
  test_OnOff_render();
  // test_flat_render();

  return 0;
}