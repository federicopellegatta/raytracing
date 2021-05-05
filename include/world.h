#ifndef WORLD_H
#define WORLD_H

#include "shapes.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

struct World {
  vector<shared_ptr<Shape>> shapes;

  void add(shared_ptr<Shape> newShape) {
    shapes.push_back(make_shared<Shape>(newShape));
  }

  HitRecord rayIntersection(Ray ray) {
    HitRecord closest{};

    for (int i{}; i < shapes.size(); i++) {
      HitRecord intersection = shapes[i]->ray_intersection(ray);
      if (!intersection.hit)
        continue;
      if ((!closest.hit) || (intersection.t < closest.t))
        HitRecord closest{intersection};
    }

    return closest;
  }
};

#endif