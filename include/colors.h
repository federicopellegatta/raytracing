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

#ifndef COLORS_H
#define COLORS_H

#include "fmt/core.h"
#include "fmtlib.h"
#include <algorithm>

using namespace std;

/**
 * @brief Function needed for tests, sets internally the precision of how close
 * two floats has to be to be considered equal
 *
 * @param a
 * @param b
 * @return true
 * @return false
 */
inline bool are_close(float a, float b) {
  float eps = 1e-5;
  return fabs(a - b) < eps;
};

/**
 * @brief Overload: pass precision as an argument
 *
 * @param a
 * @param b
 * @param eps
 * @return true
 * @return false
 */
inline bool are_close(float a, float b, float eps) {
  return fabs(a - b) < eps;
};

/**
 * @brief Struct representing a color, encoded as its three primary components
 *
 */
struct Color {
  float r, g, b;
  /**
   * @brief Construct a new Color object
   *
   * @param _r
   * @param _g
   * @param _b
   */
  Color(float _r = 0., float _g = 0., float _b = 0.) {
    r = _r;
    g = _g;
    b = _b;
  };

  /**
   * @brief Overload of plus operator between two colors
   *
   * @param a
   * @return Color
   */
  Color operator+(Color a) { return Color(r + a.r, g + a.g, b + a.b); }
  /**
   * @brief Overload of mul operator between a float and a Color
   *
   * @param a
   * @return Color
   */
  Color operator*(float a) { return Color(a * r, a * g, a * b); }
  /**
   * @brief Overload of mul operator between two Color instances
   *
   * @param a
   * @return Color
   */
  Color operator*(Color a) { return Color(a.r * r, a.g * g, a.b * b); }

  /**
   * @brief Wrapper around are_close for Color
   *
   * @param a
   * @return true
   * @return false
   */
  bool operator==(const Color a) {
    return are_close(r, a.r) && are_close(b, a.b) && are_close(g, a.g);
  }

  /**
   * @brief Wrapper around fmt::print(). For debugging purposes.
   *
   */
  inline void print() { fmt::print("r: {}, g: {}, b: {}\n", r, g, b); }

  /**
   * @brief Calculate the luminosity of a Color object
   *
   * @return float
   */
  inline float luminosity() {
    return static_cast<float>(max(max(r, g), b) + min(min(r, g), b)) / 2.;
  }
};

extern Color BLACK;
extern Color WHITE;

#endif