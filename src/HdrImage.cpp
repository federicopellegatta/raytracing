#include "../include/HdrImage.h"

using namespace std;

void HdrImage::set_pixel(unsigned int x, unsigned int y, Color color) {
  if (_valid_coordinates(x, y)) {
    pixels[_pixel_offset(x, y)] = color;
  }
}
