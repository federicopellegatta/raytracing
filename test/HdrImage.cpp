#include "../include/HdrImage.h"

using namespace std;

int main() {

  HdrImage img(7, 4);

  bool test_image_creation(HdrImage img) {
    return (img.width == 7 && img.height == 4) ? EXIT_SUCCESS : EXIT_FAILURE;
  }

  bool test_coordinates(HdrImage img) {
    if (img.valid_coordinates(0, 0) && img.valid_coordinates(6, 3) &&
        img.valid_coordinates(-1, 0) == false &&
        img.valid_coordinates(0, -1) == false) {
      return true;
    } else {
      return false;
    }
  }

  bool test_pixel_offset(HdrImage img) {

    return (img.pixel_offset(0, 0) == 0 && img.pixel_offset(3, 2) == 17 &&
            img.pixel_offset(6, 3) == 7 * 4 - 1)
               ? EXIT_SUCCESS
               : EXIT_FAILURE;
  }

  return (test_image_creation(img) && test_coordinates(img) &&
          test_pixel_offset(img))
             ? EXIT_SUCCESS
             : EXIT_FAILURE;
}