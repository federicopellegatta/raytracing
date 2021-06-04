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

  assert(tracer.image.get_pixel(0, 0) == BLACK);
  assert(tracer.image.get_pixel(1, 0) == BLACK);
  assert(tracer.image.get_pixel(2, 0) == BLACK);

  assert(tracer.image.get_pixel(0, 1) == BLACK);
  assert(tracer.image.get_pixel(1, 1) == WHITE);
  assert(tracer.image.get_pixel(2, 1) == BLACK);

  assert(tracer.image.get_pixel(0, 2) == BLACK);
  assert(tracer.image.get_pixel(1, 2) == BLACK);
  assert(tracer.image.get_pixel(2, 2) == BLACK);
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

  assert(tracer.image.get_pixel(0, 0) == BLACK);
  assert(tracer.image.get_pixel(1, 0) == BLACK);
  assert(tracer.image.get_pixel(2, 0) == BLACK);

  assert(tracer.image.get_pixel(0, 1) == BLACK);
  assert(tracer.image.get_pixel(1, 1) == sphere_color);
  assert(tracer.image.get_pixel(2, 1) == BLACK);

  assert(tracer.image.get_pixel(0, 2) == BLACK);
  assert(tracer.image.get_pixel(1, 2) == BLACK);
  assert(tracer.image.get_pixel(2, 2) == BLACK);
}

void test_pathTracer() {
  PCG pcg;

  for (int i{}; i < 5; i++) {
    World world;
    float emitted_radiance = pcg.random_float();
    float reflectance = pcg.random_float();
    Material enclosure_material{
        make_shared<DiffusiveBRDF>(
            make_shared<UniformPigment>(WHITE * reflectance)),
        make_shared<UniformPigment>(WHITE * emitted_radiance)};

    Sphere sphere(Transformation(), enclosure_material);
    world.add(make_shared<Sphere>(sphere));

    PathTracer path_tracer(world, BLACK, pcg, 1, 100, 101);

    Ray ray(Point(0, 0, 0), Vec(1, 0, 0));
    Color color = path_tracer(ray);

    float expected = emitted_radiance / (1.0 - reflectance);

    assert(are_close(expected, color.r, 1e-3));
    assert(are_close(expected, color.g, 1e-3));
    assert(are_close(expected, color.b, 1e-3));
  }
}

int main() {
  test_OnOff_render();
  test_flat_render();
  test_pathTracer();

  return 0;
}