#include "../include/HdrImage.h"

using namespace std;

HdrImage::HdrImage(unsigned int w, unsigned int h) {
  width = w;
  height = h;
}

HdrImage::~HdrImage(){};

bool HdrImage::valid_coordinates(unsigned int x, unsigned int y) {
  if (x < width && y < height) {
    return true;
  } else {
    return false;
  }
}

unsigned int HdrImage::pixel_offset(unsigned int x, unsigned int y) {
  return y * height + x;
}

void HdrImage::set_pixel(unsigned int x, unsigned int y, Color color) {
  if (valid_coordinates(x, y)) {
    pixels[pixel_offset(x, y)] = color;
  }
}

Color HdrImage::get_pixel(unsigned int x, unsigned int y) {
  return pixels[pixel_offset(x, y)];
}
