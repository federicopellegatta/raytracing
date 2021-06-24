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

#include "materials.h"
#include <cassert>

using namespace std;

void test_uniform_pigment() {
  Color color(1.0, 2.0, 3.0);
  UniformPigment pigment(color);

  assert(pigment(Vec2d(0.0, 0.0)) == (color));
  assert(pigment(Vec2d(1.0, 0.0)) == (color));
  assert(pigment(Vec2d(0.0, 1.0)) == (color));
  assert(pigment(Vec2d(1.0, 1.0)) == (color));
}

void test_checkered_pigment() {
  Color color1(1.0, 2.0, 3.0);
  Color color2(10.0, 20.0, 30.0);
  CheckeredPigment pigment(color1, color2, 2);

  assert(pigment(Vec2d(0.25, 0.25)) == (color1));
  assert(pigment(Vec2d(0.75, 0.25)) == (color2));
  assert(pigment(Vec2d(0.25, 0.75)) == (color2));
  assert(pigment(Vec2d(0.75, 0.75)) == (color1));
}

void test_image_pigment() {
  HdrImage image = HdrImage(2, 2);
  image.set_pixel(0, 0, Color(1.0, 2.0, 3.0));
  image.set_pixel(1, 0, Color(2.0, 3.0, 1.0));
  image.set_pixel(0, 1, Color(2.0, 1.0, 3.0));
  image.set_pixel(1, 1, Color(3.0, 2.0, 1.0));

  ImagePigment pigment(image);
  assert(pigment(Vec2d(0.0, 0.0)) == (Color(1.0, 2.0, 3.0)));
  assert(pigment(Vec2d(1.0, 0.0)) == (Color(2.0, 3.0, 1.0)));
  assert(pigment(Vec2d(0.0, 1.0)) == (Color(2.0, 1.0, 3.0)));
  assert(pigment(Vec2d(1.0, 1.0)) == (Color(3.0, 2.0, 1.0)));
}

/**
 * @brief `expected_coordinates` were calculated using `scatter_10000_rays.cpp`.
 * What's more, we checked that the plot of the coordinates printed out in
 * `rays_coordinates.txt` is a cosine 3D distribution
 *
 */
void test_scatter_ray() {
  PCG pcg;
  DiffusiveBRDF scatterer;
  vector<Point> expected_coordinates = {Point(-0.788602, 0.0917465, 0.608021),
                                        Point(-0.848895, -0.0799082, 0.522486),
                                        Point(0.249681, -0.82841, 0.501395),
                                        Point(-0.865152, -0.0252223, 0.500874),
                                        Point(0.935013, -0.157046, 0.317941),
                                        Point(0.332078, 0.297299, 0.895175),
                                        Point(0.184793, 0.945235, 0.269038),
                                        Point(0.31681, 0.264685, 0.910809),
                                        Point(-0.79035, -0.0945891, 0.60531),
                                        Point(-0.287109, -0.85178, 0.438224)};
  for (int i{}; i < 10; ++i) {
    Ray ray = scatterer.scatter_ray(pcg, Vec(), Point(), VEC_Z.to_norm(), 2);
    float x_ray = ray.origin.x + ray.dir.x;
    float y_ray = ray.origin.y + ray.dir.y;
    float z_ray = ray.origin.z + ray.dir.z;
    Point p(x_ray, y_ray, z_ray);
    assert(p == expected_coordinates.at(i));
  }
}

int main() {
  test_uniform_pigment();
  test_checkered_pigment();
  test_image_pigment();
  test_scatter_ray();
  return 0;
}