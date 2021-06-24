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

void ImageTracer::fire_all_rays(function<Color(const Ray &)> func) {
  for (int row{}; row < image.height; row++) {
    for (int col{}; col < image.width; col++) {
      Color cum_color;

      if (samples_per_side > 0) {
        for (int inter_pixel_row{}; inter_pixel_row < samples_per_side;
             ++inter_pixel_row) {
          for (int inter_pixel_col{}; inter_pixel_col < samples_per_side;
               ++inter_pixel_col) {
            float u_pixel =
                (inter_pixel_col + pcg.random_float()) / samples_per_side;
            float v_pixel =
                (inter_pixel_row + pcg.random_float()) / samples_per_side;
            Ray ray = fire_ray(col, row, u_pixel, v_pixel);
            cum_color = cum_color + func(ray);
          }
        }
        image.set_pixel(col, row, cum_color * (1. / pow(samples_per_side, 2)));
      } else {
        Ray ray = fire_ray(col, row);
        image.set_pixel(col, row, func(ray));
      }
    }
  }
}