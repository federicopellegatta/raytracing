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

#include "pcg.h"
#include <cassert>

void test_random() {
  PCG pcg;
  assert(pcg.state == 1753877967969059832);
  assert(pcg.inc == 109);

  uint32_t expected[6] = {2707161783, 2068313097, 3122475824,
                          2211639955, 3215226955, 3421331566};

  for (int i{}; i < 6; i++)
    assert(expected[i] == pcg.random());
}

int main() {
  test_random();

  return 0;
}