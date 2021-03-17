#include "../include/HdrImage.h"

using namespace std;

HdrImage::HdrImage(unsigned int w, unsigned int h) {
  width = w;
  height = h;
}

HdrImage::~HdrImage(){};

bool HdrImage::_valid_coordinates(unsigned int x, unsigned int y) {
  if (x < width && y < height) {
    return true;
  } else {
    return false;
  }
}

unsigned int HdrImage::_pixel_offset(unsigned int x, unsigned int y) {
  return y * height + x;
}

void HdrImage::set_pixel(unsigned int x, unsigned int y, Color color) {
  if (_valid_coordinates(x, y)) {
    pixels[_pixel_offset(x, y)] = color;
  }
}