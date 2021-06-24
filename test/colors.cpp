/*
 * Copyright (c) 2021 Simone Pirota, Federico Pellegatta
 *
 * This file is part of raytracer.
 *
 * raytracer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * raytracer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with raytracer.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "colors.h"
#include <cassert>
#include <cstdlib>

int main() {
  Color c1(1.0, 2.0, 3.0);
  Color c2(5.0, 7.0, 9.0);
  float a = 3.0;

  Color c3(6.0, 9.0, 12.0);
  Color c4(3.0, 6.0, 9.0);
  Color c5(5.0, 14.0, 27.0);

  assert(c3 == c1 + c2); // check colors sum
  assert(c4 == c1 * a);  // check product color by float
  assert(c5 == c1 * c2); // check product color by color

  assert(are_close(2.0, c1.luminosity()));
  assert(are_close(7.0, c2.luminosity()));

  return 0;
}