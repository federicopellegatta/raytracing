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