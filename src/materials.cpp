#include "materials.h"

Color CheckeredPigment::get_color(Vec2d uv) {
  int int_u = int(floor(uv.u * num_of_steps));
  int int_v = int(floor(uv.v * num_of_steps));

  return ((int_u % 2) == (int_v % 2)) ? color1 : color2;
}

Color ImagePigment::get_color(Vec2d uv) {
  int col = int(uv.u * image.width);
  int row = int(uv.v * image.height);

  if (col >= image.width)
    col = image.width - 1;

  if (row >= image.height)
    row = image.height - 1;

  return image.get_pixel(col, row);
}
