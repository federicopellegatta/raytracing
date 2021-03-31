// Color header file
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

bool are_close(float, float);
