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

#ifndef RAY_H
#define RAY_H

#include "geometry.h"
#include <iostream>
#include <limits>

using namespace std;

/** Ray class
 * @brief A ray of light propagating in space
 *
 * @param origin A `Point` where the ray is originated
 * @param dir A `Vec`that indicates the direction along which the ray propagates
 * @param depth number of times this ray was reflected/refracted
 * @param tmin The minimum distance travelled by the ray is this float number
 * @param tmax The maximum distance travelled by the ray is this float number
 *
 * @see Point
 * @see Vec
 */
struct Ray {
  Point origin;
  Vec dir;
  float tmin;
  float tmax;
  int depth;

  /**
   * @brief Construct a new Ray object
   *
   * @param _origin
   * @param _dir
   * @param _depth
   * @param _tmin
   * @param _tmax
   */
  Ray(Point _origin = Point(), Vec _dir = Vec(), int _depth = 0,
      float _tmin = 1e-5, float _tmax = std::numeric_limits<float>::infinity())
      : origin{_origin}, dir{_dir} {
    depth = _depth;
    tmin = _tmin;
    tmax = _tmax;
  }

  /**
   * @brief Check wether two Rays are the same or not
   *
   * @param other
   * @return true
   * @return false
   */
  bool operator==(Ray other) {
    return (origin == other.origin) && (dir == other.dir);
  }
  bool operator!=(Ray other) { return !(*this == other); }

  /**
   * @brief Compute the point along the ray's path at some distance from the
   * origin
   *
   * @param t Length in `dir` unit
   * @return The point in 3D space whose distance from the
        ray's origin is equal to `t`
   */
  inline Point at(float t) { return origin + dir * t; }

  /**
   * @brief Transform a ray
   * This method returns a new ray whose origin and direction are the
   transformation of the original ray
   *
   * @param transformation
   * @return The ray transformed
   */
  inline Ray transform(Transformation transformation) {
    return Ray(transformation * origin, transformation * dir, depth, tmin,
               tmax);
  }
};
#endif