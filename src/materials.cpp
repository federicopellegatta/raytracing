#include "materials.h"

Color CheckeredPigment::get_color(Vec2d uv) {
  int int_u = int(floor(uv.u * num_of_steps));
  int int_v = int(floor(uv.v * num_of_steps));

  return ((int_u % 2) == (int_v % 2)) ? color1 : color2;
}