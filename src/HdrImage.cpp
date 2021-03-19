#include "../include/HdrImage.h"
#include <iostream>

using namespace std;

HdrImage::HdrImage(int w, int h) : pixels(w * h) {
  width = w;
  height = h;
}

HdrImage::~HdrImage(){};

bool HdrImage::valid_coordinates(int x, int y) {
  if (x < width && y < height) {
    return true;
  } else {
    return false;
  }
}

int HdrImage::pixel_offset(int x, int y) { return y * width + x; }

void HdrImage::set_pixel(int x, int y, Color color) {
  if (valid_coordinates(x, y)) {
    pixels[pixel_offset(x, y)] = color;
  } else {
    cout << "Couldn't set pixel because coordinates are not valid!" << endl;
  }
}

Color HdrImage::get_pixel(int x, int y) { return pixels[pixel_offset(x, y)]; }
