#include "render.h"

Color FlatRenderer::operator()(Ray ray) {
  HitRecord intersection = world.ray_intersection(ray);
  if (!intersection.hit)
    return background_color;

  Material material = intersection.shape->material;
  return (material.brdf->pigment->get_color(intersection.surface_point) +
          material.emitted_radiance->get_color(intersection.surface_point));
}