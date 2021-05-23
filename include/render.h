#ifndef RENDER_H
#define RENDER_H

#include "colors.h"
#include "geometry.h"
#include "materials.h"
#include "ray.h"
#include "world.h"

/**
 * @brief A class implementing a solver of the rendering equation
 * This is an abstract class.
 *
 */
struct Renderer {
  World world;
  Color background_color;

  Renderer(World _world, Color _background_color = BLACK)
      : world{_world}, background_color{_background_color} {};

  /**
   * @brief Estimante a radiance along a Ray
   *
   * @return Color
   */
  virtual Color operator()(Ray) = 0;
};

/**
 * @brief An on/off renderer, mostly useful for debugging purposes, because it
 * is really fast but produce only black and white images
 *
 */
struct OnOffRenderer : public Renderer {
  Color color;
  OnOffRenderer(World world, Color background_color = BLACK,
                Color _color = WHITE)
      : Renderer(world, background_color), color{_color} {};

  Color operator()(Ray ray) {
    return world.ray_intersection(ray).hit ? color : background_color;
  }
};

/**
 * @brief A "flat" renderer, that estimates the solution of the rendering
 equation by neglecting any contribution of the light. It just uses the pigment
 of each surface to determine how to compute the final radiance.
 *
 */
struct FlatRenderer : public Renderer {
  FlatRenderer(World world, Color background_color = BLACK)
      : Renderer(world, background_color){};

  Color operator()(Ray ray);
};
#endif