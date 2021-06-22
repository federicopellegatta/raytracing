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

#include "imagetracer.h"
#include <cassert>

using namespace std;

void test_uv_sub_mapping(ImageTracer tracer) {
  Ray ray1 = tracer.fire_ray(0, 0, 2.5, 1.5);
  Ray ray2 = tracer.fire_ray(2, 1, 0.5, 0.5);
  assert(ray1 == ray2);
}

void test_orientation(ImageTracer tracer) {
  Ray top_left_ray = tracer.fire_ray(0, 0, 0.0, 0.0);
  assert(Point(0.0, 2.0, 1.0) == top_left_ray.at(1.0));

  // Fire a ray against bottom - right corner of the screen
  Ray bottom_right_ray = tracer.fire_ray(3, 1, 1.0, 1.0);
  assert(Point(0.0, -2.0, -1.0) == bottom_right_ray.at(1.0));
}

void test_image_coverage(ImageTracer tracer) {
  tracer.fire_all_rays([](Ray ray) -> Color { return Color(1.0, 2.0, 3.0); });
  for (int row{}; row < tracer.image.height; row++) {
    for (int col{}; col < tracer.image.width; col++) {
      assert(tracer.image.get_pixel(col, row) == (Color(1.0, 2.0, 3.0)));
    }
  }
}
int main() {

  HdrImage img(4, 2);
  shared_ptr<Camera> camera =
      make_shared<PerspectiveCamera>(1.0, 2.0, Transformation());
  ImageTracer tracer(img, camera);

  test_uv_sub_mapping(tracer);
  test_image_coverage(tracer);
  test_orientation(tracer);

  return 0;
}