#include "../include/HdrImage.h"

using namespace std;

int main() {
  HdrImage img(7, 4);
  bool test_set_get_pixel;
  Color reference_color(1.0, 2.0, 3.0);

  // Testing if set and get pixel works properly
  img.set_pixel(3, 2, reference_color);
  test_set_get_pixel = reference_color.is_close(img.get_pixel(3, 2));

  return (test_set_get_pixel) ? EXIT_SUCCESS : EXIT_FAILURE;
}