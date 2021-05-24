#include "materials.h"
#include <cassert>

using namespace std;

void test_uniform_pigment() {
  Color color(1.0, 2.0, 3.0);
  UniformPigment pigment(color);

  assert(pigment(Vec2d(0.0, 0.0)) == (color));
  assert(pigment(Vec2d(1.0, 0.0)) == (color));
  assert(pigment(Vec2d(0.0, 1.0)) == (color));
  assert(pigment(Vec2d(1.0, 1.0)) == (color));
}

void test_checkered_pigment() {
  Color color1(1.0, 2.0, 3.0);
  Color color2(10.0, 20.0, 30.0);
  CheckeredPigment pigment(color1, color2, 2);

  assert(pigment(Vec2d(0.25, 0.25)) == (color1));
  assert(pigment(Vec2d(0.75, 0.25)) == (color2));
  assert(pigment(Vec2d(0.25, 0.75)) == (color2));
  assert(pigment(Vec2d(0.75, 0.75)) == (color1));
}

void test_image_pigment() {
  HdrImage image = HdrImage(2, 2);
  image.set_pixel(0, 0, Color(1.0, 2.0, 3.0));
  image.set_pixel(1, 0, Color(2.0, 3.0, 1.0));
  image.set_pixel(0, 1, Color(2.0, 1.0, 3.0));
  image.set_pixel(1, 1, Color(3.0, 2.0, 1.0));

  ImagePigment pigment(image);
  assert(pigment(Vec2d(0.0, 0.0)) == (Color(1.0, 2.0, 3.0)));
  assert(pigment(Vec2d(1.0, 0.0)) == (Color(2.0, 3.0, 1.0)));
  assert(pigment(Vec2d(0.0, 1.0)) == (Color(2.0, 1.0, 3.0)));
  assert(pigment(Vec2d(1.0, 1.0)) == (Color(3.0, 2.0, 1.0)));
}

int main() {
  test_uniform_pigment();
  test_checkered_pigment();
  test_image_pigment();
  return 0;
}