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

PCG::PCG(uint64_t init_state, uint64_t init_seq) {
  inc = (init_seq << 1) | 1;
  random(); // Throw a random number and discard it
  state += init_state;
  random(); // Throw a random number and discard it
}

uint32_t PCG::random() {
  uint64_t old_state = state;
  state = old_state * 6364136223846793005 + inc;

  uint32_t xorshifted = ((old_state >> 18) ^ old_state) >> 27;
  uint32_t rot = old_state >> 59;

  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}