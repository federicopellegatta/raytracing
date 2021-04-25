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

// Color::operator*(Color b, float a) { return Color(a * b.r, a * b.g, a * b.b);
// }

Color Color::operator*(Color a) { return Color(a.r * r, a.g * g, a.b * b); }

bool Color::is_close(Color a) {
  return are_close(r, a.r) && are_close(b, a.b) && are_close(g, a.g);
}

void Color::print() {
  std::cout << "r: " << r << ", g: " << g << ", b: " << b << std::endl;
}

float Color::luminosity() {
  return float(max(max(r, g), b) + min(min(r, g), b)) / 2;
}

bool are_close(float a, float b) {
  float eps = 1e-5;
  return abs(a - b) < eps;
}

bool are_close(float a, float b, float eps) { return abs(a - b) < eps; }