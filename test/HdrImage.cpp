#include "../include/HdrImage.h"
#include <cassert>

using namespace std;

void test_image_creation(HdrImage img, unsigned int x, unsigned int y) {
  assert(img.width == x);
  assert(img.height == y);
}

void test_coordinates(HdrImage img) {
  assert(img.valid_coordinates(0, 0));
  assert(img.valid_coordinates(6, 3));
  assert(img.valid_coordinates(-1, 0) == false);
  assert(img.valid_coordinates(0, -1) == false);
}

void test_pixel_offset(HdrImage img) {
  assert(img.pixel_offset(0, 0) == 0);
  assert(img.pixel_offset(3, 2) == 17);
  assert(img.pixel_offset(6, 3) == 7 * 4 - 1);
}

void test_set_get_pixel(HdrImage img, Color color, unsigned int x,
                        unsigned int y) {
  assert(color.is_close(img.get_pixel(x, y)));
}

int main() {

  HdrImage img(7, 4);
  Color reference_color(1.0, 2.0, 3.0);

  // Testing if set and get pixel works properly
  img.set_pixel(3, 2, reference_color);

  test_image_creation(img, 7, 4);
  test_coordinates(img);
  test_pixel_offset(img);
  test_set_get_pixel(img, reference_color, 3, 2);

  return 0;
}