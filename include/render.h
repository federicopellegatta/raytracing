#ifndef RENDER_H
#define RENDER_H

#include "colors.h"
#include "geometry.h"
#include "materials.h"
#include "ray.h"
#include "world.h"

struct Renderer {
  World world;
  Color background_color;

  Renderer(World _world, Color _background_color = BLACK)
      : world{_world}, background_color{_background_color} {};

  virtual Color operator()(Ray) = 0;
};

struct OnOffRenderer : public Renderer {
  Color color;
  OnOffRenderer(World world, Color background_color = BLACK,
                Color _color = WHITE)
      : Renderer(world, background_color) {
    color = _color;
  };

  Color operator()(Ray ray) {
    return world.ray_intersection(ray).hit ? color : background_color;
  }
};

struct FlatRenderer : public Renderer {
  FlatRenderer(World world, Color background_color = BLACK)
      : Renderer(world, background_color){};

  Color operator()(Ray ray);
};

Color FlatRenderer::operator()(Ray ray) {
  HitRecord intersection = world.ray_intersection(ray);
  if (!intersection.hit)
    return background_color;

  Material material = intersection.shape->material;
  return (material.brdf->pigment->get_color(intersection.surface_point) +
          material.emitted_radiance->get_color(intersection.surface_point));
}
#endif