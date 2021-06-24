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

#include "world.h"
#include "ray.h"

HitRecord World::ray_intersection(Ray ray) {
  HitRecord closest{shapes.at(0)};
  for (int i{}; i < shapes.size(); i++) {
    HitRecord intersection = shapes[i]->ray_intersection(ray);
    if (!intersection.hit)
      continue;
    if ((!closest.hit) || (intersection.t < closest.t)) {
      closest = intersection;
    }
  }
  return closest;
}