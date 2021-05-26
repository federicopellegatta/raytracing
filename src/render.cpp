#include "render.h"

Color FlatRenderer::operator()(Ray ray) {
  HitRecord intersection = world.ray_intersection(ray);
  if (!intersection.hit)
    return background_color;

  Material material = intersection.shape->material;
  return ((*(material.brdf->pigment))(intersection.surface_point) +
          (*material.emitted_radiance)(intersection.surface_point));
}

Color PathTracer::operator()(Ray ray) {
  if (ray.depth > max_depth)
    return BLACK;

  HitRecord intersection = world.ray_intersection(ray);
  if (!intersection.hit)
    return background_color;

  Material hit_material = intersection.shape->material;
  Color hit_color = (*(hit_material.brdf->pigment))(intersection.surface_point);
  Color emitted_radiance =
      (*hit_material.emitted_radiance)(intersection.surface_point);

  float hit_color_lum =
      max(max(hit_color.r, hit_color.g),
          hit_color.b); // q probability needed in russian roulette

  // Russian Roulette
  if (ray.depth >= russian_roulette_limit) {
    if (pcg.random_float() > hit_color_lum)
      // Keep the recursion going, but compensate for other potentially
      // discarded rays
      hit_color = hit_color * (1.0 / (1.0 - hit_color_lum));
    else {
      // Terminate prematurely
      return emitted_radiance;
    }
  }

  Color cum_radiance = BLACK;
  if (hit_color_lum > 0.0) { // Only do costly recursions if it's worth it
    for (int i{}; i < num_of_rays; i++) {
      Ray new_ray = hit_material.brdf.scatter_ray(
          pcg, intersection.ray.dir, intersection.world_point,
          intersection.normal, ray.depth + 1);

      // Recursive call
      Color new_radiance = (*this)(new_ray);
      Color cum_radiance = cum_radiance + (hit_color * new_radiance);

      return emitted_radiance + cum_radiance * (1.0 / num_of_rays);
    }
  }
}