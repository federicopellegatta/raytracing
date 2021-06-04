#ifndef RENDER_H
#define RENDER_H

#include "colors.h"
#include "geometry.h"
#include "materials.h"
#include "pcg.h"
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

/**
 * @brief A simple path-tracing renderer
 *
 * The algorithm implemented here allows the caller to tune number of rays
 * thrown at each iteration, as well as the maximum depth. It implements Russian
 * roulette, so in principle it will take a finite time to complete the
 * calculation even if you set max_depth to `math.inf`.
 */
struct PathTracer : public Renderer {
  PCG pcg;
  int num_of_rays, max_depth, russian_roulette_limit;

  /**
   * @brief Construct a new Path Tracing object
   *
   * @param world
   * @param background_color
   * @param pcg
   * @param num_of_rays
   * @param max_depth
   * @param russian_roulette_limit
   */
  PathTracer(World world, Color background_color, PCG _pcg = PCG(),
             int _num_of_rays = 10, int _max_depth = 2,
             int _russian_roulette_limit = 3)
      : Renderer(world, background_color), pcg{_pcg} {
    num_of_rays = _num_of_rays;
    max_depth = _max_depth;
    russian_roulette_limit = _russian_roulette_limit;
  }

  Color operator()(Ray ray);
};
#endif