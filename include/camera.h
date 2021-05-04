#include "ray.h"

/** Camera Class
 * @brief Abstract class representing an observer.
 * Base class for OrtogonalCamera and PerspectiveCamera
 *
 * @see OrthogonalCamera
 * @see PerspectiveCamera
 */
struct Camera {

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
 * @param transformation The transformation to be applied to the camera (default
 * identity).
 * @param aspect_ratio A float number representing the aspect ratio of the
 * screen (usually 16:9)
 */
struct OrthogonalCamera : public Camera {
  Transformation transformation;
  float aspect_ratio;

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
      : aspect_ratio{_aspect_ratio}, transformation{_transformation} {}

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
   * @param u `u` coordiante in screen frame
   * @param v `v` coordiante in screen frame
   * @return Ray generated passing through (u, v)
   */
  Ray fire_ray(float u, float v) {
    Point origin(-1.0, (1.0 - 2 * u) * aspect_ratio, 2 * v - 1.0);
    Vec dir = VEC_X;
    return Ray{origin, dir, 0, 1.0}.transform(transformation);
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
 * @param aspect_ratio A float number representing the aspect ratio of the
 * screen (usually 16:9)
 * @param transformation The transformation to be applied to the camera
 * (default identity)
 *
 * @see Transformation
 */
struct PerspectiveCamera : public Camera {
  Transformation transformation;
  float aspect_ratio, screen_distance;

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
      : screen_distance{_screen_distance}, aspect_ratio{_aspect_ratio},
        transformation{_transformation} {}

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
   * @param u `u` coordiante in screen frame
   * @param v `v` coordiante in screen frame
   * @return Ray generated passing through (u, v)
   */
  Ray fire_ray(float u, float v) {
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
  float aperture_deg() {
    return 2.f * atan(screen_distance / aspect_ratio) * 180.f / M_PI;
  }
};
