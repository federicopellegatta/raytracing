// Color header file
#ifndef COLORS_H
#define COLORS_H

#include "fmt/core.h"
#include "fmtlib.h"
#include <algorithm>

struct Color {
public:
  float r, b, g;

  Color();
  Color(float, float, float);
  ~Color();

  Color operator+(Color);
  Color operator*(float);
  // Color operator*(Color, float);
  Color operator*(Color);

  bool is_close(Color);

  void print();

  float luminosity();
};

inline bool are_close(float a, float b) {
  float eps = 1e-5;
  return abs(a - b) < eps;
};
inline bool are_close(float a, float b, float eps) { return abs(a - b) < eps; };

#endif