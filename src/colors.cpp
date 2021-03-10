// src/colors.cpp

#include "../include/colors.h"
#include <iostream>

using namespace std;

int main() {

  bool Color::is_close(Color b) {
    return Color::are_close(this.r, b.r) && Color::are_close(this.b, b.b) &&
           Color::are_close(this.g, b.g);
  }

  bool Color::are_close(float a, float b) {
    float eps = 1e-10;
    return abs(a, b) < eps;
  }
}
