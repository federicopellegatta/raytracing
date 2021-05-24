#include "materials.h"

Color CheckeredPigment::operator()(Vec2d uv) {
  int int_u = static_cast<int>(floor(uv.u * num_of_steps));
  int int_v = static_cast<int>(floor(uv.v * num_of_steps));

  return ((int_u % 2) == (int_v % 2)) ? color1 : color2;
}

Color ImagePigment::operator()(Vec2d uv) {
  int col = static_cast<int>(uv.u * image.width);
  int row = static_cast<int>(uv.v * image.height);

  if (col >= image.width)
    col = image.width - 1;

  if (row >= image.height)
    row = image.height - 1;

  return image.get_pixel(col, row);
}
