#include "../include/colors.h"
#include <cstdlib>
#include <iostream>

using namespace std;

Color::Color() {
  r = 0;
  g = 0;
  b = 0;
}
Color::Color(float R, float G, float B) {
  r = R;
  g = G;
  b = B;
}

Color::~Color(){};

Color Color::operator+(Color a) { return Color(r + a.r, g + a.g, b + a.b); }

Color Color::operator*(float a) { return Color(a * r, a * g, a * b); }

Color Color::operator*(Color a) { return Color(a.r * r, a.g * g, a.b * b); }

bool Color::operator==(Color a) {
  return are_close(r, a.r) && are_close(b, a.b) && are_close(g, a.g);
}

void Color::print() {
  std::cout << "r: " << r << ", g: " << g << ", b: " << b << std::endl;
}

float Color::luminosity() {
  return float(max(max(r, g), b) + min(min(r, g), b)) / 2;
}

Color WHITE(1.0, 1.0, 1.0);
Color BLACK(0.0, 0.0, 0.0);