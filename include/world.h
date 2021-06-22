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

#ifndef WORLD_H
#define WORLD_H

#include "shapes.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;
/**
 * @brief A class holding a list of shapes, which make a "world"
    You can add shapes to a world using add. Typically, you call
    ray_intersection to check whether a light ray intersects any
    of the shapes in the world.
 *
 */
struct World {
  vector<shared_ptr<Shape>> shapes;

  /**
   * @brief Add a new shape to the world
   *
   * @param newShape
   */
  inline void add(shared_ptr<Shape> newShape) { shapes.push_back(newShape); }
  /**
   * @brief Determine if a ray intersect an object of the current world.
   *
   * @param ray
   * @return HitRecord
   */
  HitRecord ray_intersection(Ray ray);
};

#endif