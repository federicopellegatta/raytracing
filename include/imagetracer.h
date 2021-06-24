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

#ifndef IMAGETRACER_H
#define IMAGETRACER_H

#include "HdrImage.h"
#include "camera.h"
#include "colors.h"
#include "pcg.h"
#include "render.h"
#include <functional>

using namespace std;

/** ImageTracer Class
 * @brief Trace an image by shooting light rays through each of its pixels
 *
 * @param image A :class:`.HdrImage` object; it must be already initialized
 * @param camera must be a descendant of the :class:`.Camera` object
 * (:class:`.OrthogonalCamera` or :class:`.PerspectiveCamera`)
 *
 * @see HdrImage
 * @see Camera
 * @see OrthogonalCamera
 * @see PerspectiveCamera
 */
struct ImageTracer {
  HdrImage image;
  shared_ptr<Camera> camera;
  int samples_per_side;
  PCG pcg;

  /**
   * @brief Construct a new Image Tracer object
   * If `samples_per_side` is greater than 0, then stratified sampling will be
   applied to each pixel in the image, using the random number generator `pcg`.
   *
   * @param _image
   * @param _camera
   * @param _samples_per_side
   * @param _pcg
   */
  ImageTracer(HdrImage _image, shared_ptr<Camera> _camera,
              int _samples_per_side = 0, PCG _pcg = PCG())
      : image{_image}, camera{_camera}, pcg{_pcg} {
    samples_per_side = _samples_per_side;
  }

  /**
   * @brief Shoot one light ray through image pixel (col, row)
   *
   * The parameters (col, row) are measured in the same way as they are in
   :class:`.HdrImage`: the bottom left corner is placed at (0, 0).
   The values of `u_pixel` and `v_pixel` are floating-point numbers in the range
   [0, 1]. They specify where the ray should cross the pixel; passing 0.5 to
   both means that the ray will pass through the pixel's center.
   *
   * @param col image column
   * @param row image row
   * @param u_pixel `u` coordinate in pixel frame
   * @param v_pixel `v` coordinate in pixel frame
   * @return the Ray passing through (col, row) starting from the observer
   */
  inline Ray fire_ray(int col, int row, float u_pixel = 0.5,
                      float v_pixel = 0.5) {
    float u = (col + u_pixel) / image.width;
    float v = 1.0 - (row + v_pixel) / image.height;

    return camera->fire_ray(u, v);
  }

  /**
   * @brief Shoot several light rays crossing each of the pixels in the image
   *
   * For each pixel in the :class:`.HdrImage` object fire one ray, and pass it
   to the function `func`
   *
   * @param func It must accept :class:`.Ray` as its only parameter and must
   return a :class:`.Color` instance telling the color to
   assign to that pixel in the image.
   */
  void fire_all_rays(function<Color(const Ray &)>);
};
#endif