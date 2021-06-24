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

#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

/** Camera Class
 * @brief Abstract class representing an observer.
 * Base class for OrtogonalCamera and PerspectiveCamera
 *
 * @param transformation The transformation to be applied to the camera (default
 * identity).
 * @param aspect_ratio A float number representing the aspect ratio of the
 * screen (usually 16:9)
 * @see OrthogonalCamera
 * @see PerspectiveCamera
 */
struct Camera {
  Transformation transformation;
  float aspect_ratio;
  /**
   * @brief Construct a new Camera object
   *
   * @param _transformation
   */
  Camera(float _aspect_ratio = 1.f,
         Transformation _transformation = Transformation())
      : transformation{_transformation} {
    aspect_ratio = _aspect_ratio;
  }
  /**
   * @brief Fire a ray (or all of them) through a camera
   * Implementation is to be provided by OrtogonalCamera and PerspectiveCamera
   */
  virtual Ray fire_ray(float, float) = 0;
};

/** OrthogonalCamera class
 * @brief A camera implementing an ortogonal 3D -> 2D projection
 * This class implements an observer seeing the world through an orthogonal
 * projection.
 *
 */
struct OrthogonalCamera : public Camera {
  /**
   * @brief Construct a new Orthogonal Camera object
   *
   * @param _aspect_ratio defines how larger than the height the image is
   * (usually 16:9)
   * @param _transformation The transformation to be applied to the camera
   * (default identity).
   *
   * @see Tranformation
   */
  OrthogonalCamera(float _aspect_ratio = 1.f,
                   Transformation _transformation = Transformation())
      : Camera{_aspect_ratio, _transformation} {}

  /**
   * @brief Shoots a ray through the camera screen
   * The coordinates (u, v) specify the point on the screen where the ray
   crosses it.
   * The screen coordinates can be seen in the following diagram
       (0, 1)                          (1, 1)
          +------------------------------+
          |                              |
          |                              |
          |                              |
          +------------------------------+
       (0, 0)                          (1, 0)
   * @param u `u` coordinate in screen frame
   * @param v `v` coordinate in screen frame
   * @return Ray generated passing through (u, v)
   */
  inline Ray fire_ray(float u, float v) {
    Point origin(-1.0, (1.0 - 2 * u) * aspect_ratio, 2 * v - 1.0);
    return Ray{origin, VEC_X, 0, 1.0}.transform(transformation);
  }
};

/** PerspectiveCamera class
 * @brief A camera implementing a perspective 3D -> 2D projection.
 * This class implements an observer seeing the world through a perspective
 * projection.
 *
 * @param _screen_distance A float number which indicate how much the eye of the
 * observer is distant from the screen, and it influences the "aperture" (field
 * of view angle along the horizontal direction)
 *
 * @see Transformation
 */
struct PerspectiveCamera : public Camera {
  float screen_distance;

  /**
   * @brief Construct a new Perspective Camera object
   *
   * @param _screen_distance tells how much the eye of the observer is distant
   * from the screen, and it influences the "aperture" (field of view angle
   * along the horizontal direction)
   * @param _aspect_ratio defines how larger than the height the image is
   * @param _transformation instance of the Transformation class
   *
   * @see Transformation
   */
  PerspectiveCamera(float _screen_distance = 1.f, float _aspect_ratio = 1.f,
                    Transformation _transformation = Transformation())
      : Camera{_aspect_ratio, _transformation} {
    screen_distance = _screen_distance;
  }

  /**
   * @brief Shoots a ray through the camera screen
   * The coordinates (u, v) specify the point on the screen where the ray
   crosses it.
   * The screen coordinates can be seen in the following diagram
       (0, 1)                          (1, 1)
          +------------------------------+
          |                              |
          |                              |
          |                              |
          +------------------------------+
       (0, 0)                          (1, 0)
   * @param u `u` coordinate in screen frame
   * @param v `v` coordinate in screen frame
   * @return Ray generated passing through (u, v)
   */
  inline Ray fire_ray(float u, float v) {
    Point origin(-screen_distance, 0.f, 0.f);
    Vec direction(screen_distance, (1.0 - 2 * u) * aspect_ratio, 2 * v - 1.0);
    return Ray(origin, direction, 1.0).transform(transformation);
  }

  /**
   * @brief Compute the aperture of the camera in degrees, which is the angle of
   * the field-of-view along the horizontal direction (Y axis)
   *
   * @return The aperture of the camera in degrees
   */
  inline float aperture_deg() {
    return 2.f * atan(screen_distance / aspect_ratio) * 180.f / M_PI;
  }
};
#endif