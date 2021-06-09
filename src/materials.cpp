#include "materials.h"

Color CheckeredPigment::operator()(Vec2d uv) {
  int int_u = static_cast<int>(floor(uv.u * num_of_steps));
  int int_v = static_cast<int>(floor(uv.v * num_of_steps));

  return ((int_u % 2) == (int_v % 2)) ? color1 : color2;
}

Color ImagePigment::operator()(Vec2d uv) {
  int col = static_cast<int>(uv.u * image.width);
  int row = static_cast<int>(uv.v * image.height);

  if (col >= image.width)
    col = image.width - 1;

  if (row >= image.height)
    row = image.height - 1;

  return image.get_pixel(col, row);
}

Ray DiffusiveBRDF::scatter_ray(PCG &pcg, Vec inc_dir, Point interaction_point,
                               Normal normal, int depth) {
  ONB orthonormal_base(normal);
  float cos_theta_sq = pcg.random_float();
  float cos_theta = sqrt(cos_theta_sq), sin_theta = sqrt(1 - cos_theta_sq);
  float phi = 2. * M_PI * pcg.random_float();
  Vec dir(orthonormal_base.e1 * cos(phi) * cos_theta +
          orthonormal_base.e2 * sin(phi) * cos_theta +
          orthonormal_base.e3 * sin_theta);
  return Ray(interaction_point, dir, depth, 1e-3);
}

/**
 * @brief No need to use a PCG obj, because the reflected direction for a
 * perfect mirror is always deterministic
 *
 * @param pcg
 * @param inc_dir
 * @param interaction_point
 * @param normal
 * @param depth
 * @return Ray
 */
Ray SpecularBRDF::scatter_ray(PCG &pcg, Vec inc_dir, Point interaction_point,
                              Normal normal, int depth) {

  inc_dir.normalize();
  Vec ray_dir = inc_dir;
  Vec normal_vec = normal.to_vec();
  normal_vec.normalize();
  return Ray(interaction_point,
             Vec(ray_dir - (normal_vec * 2 * normal_vec.dot(ray_dir))), depth);
}