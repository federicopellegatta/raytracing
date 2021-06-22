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

#ifndef PCG_H
#define PCG_H

#include <cstdint>

/**
 * @brief PCG Uniform Pseudo-random Number Generator
 *
 */
struct PCG {
  uint64_t state = 0;
  uint64_t inc = 0;

  /**
   * @brief Construct a new PCG object
   *
   * @param init_state The seed of generator
   * @param init_seq The sequence ID
   */
  PCG(uint64_t init_state = 42, uint64_t init_seq = 54);

  /**
   * @brief Return a new random number and advance PCG's internal state
   *
   * @return a new `int` random number in [0, 2^32-1]
   */
  uint32_t random();

  /**
   * @brief Return a new random number uniformly distributed over [0, 1]
   *
   * @return A random `float` number in [0,1]
   */
  float random_float() { return random() / static_cast<float>(0xffffffff); }
};

#endif