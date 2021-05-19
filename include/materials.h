#ifndef MATERIALS_H
#define MATERIALS_H

#include "HdrImage.h"
#include "colors.h"
#include "geometry.h"
#include "shapes.h"

struct Pigment {
  virtual Color get_color(Vec2d) = 0;
};

struct UniformPigment : public Pigment {
  Color color;
  UniformPigment(Color _color = Color()) : color{_color} {}
  Color get_color(Vec2d uv) { return color; }
};

struct CheckeredPigment : public Pigment {
  Color color1, color2;
  int num_of_steps;

  CheckeredPigment(Color _color1 = Color(), Color _color2 = Color(),
                   int _num_of_steps = 10)
      : color1{_color1}, color2{_color2}, num_of_steps{_num_of_steps} {}
  Color get_color(Vec2d uv);
};
#endif