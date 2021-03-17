#include "../include/HdrImage.h"

using namespace std;

bool test_image_creation(HdrImage img, unsigned int x, unsigned int y) {
  return (img.width == x && img.height == y);
}

bool test_coordinates(HdrImage img) {
  return (img.valid_coordinates(0, 0) && img.valid_coordinates(6, 3) &&
          img.valid_coordinates(-1, 0) == false &&
          img.valid_coordinates(0, -1) == false);
}

bool test_pixel_offset(HdrImage img) {
  return (img.pixel_offset(0, 0) == 0 && img.pixel_offset(3, 2) == 17 &&
          img.pixel_offset(6, 3) == 7 * 4 - 1);
}

bool test_set_get_pixel(HdrImage img, Color color, unsigned int x,
                        unsigned int y) {
  return (color.is_close(img.get_pixel(x, y)));
}

int main() {

  HdrImage img(7, 4);
  Color reference_color(1.0, 2.0, 3.0);

  // Testing if set and get pixel works properly
  img.set_pixel(3, 2, reference_color);

  return (test_image_creation(img, 7, 4) && test_coordinates(img) &&
          test_pixel_offset(img) &&
          test_set_get_pixel(img, reference_color, 3, 2))
             ? EXIT_SUCCESS
             : EXIT_FAILURE;
}