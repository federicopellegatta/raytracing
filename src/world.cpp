#include "world.h"
#include "ray.h"

HitRecord World::ray_intersection(Ray ray) {
  HitRecord closest{shapes[0]};
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
