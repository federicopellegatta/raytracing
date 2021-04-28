#include "ray.h"

/**
 * @brief Abstract class representing an observer.
 * Base class for OrtogonalCamera and PerspectiveCamera
 *
 */
struct Camera {

  /**
   * @brief Fire a ray (or all of them) through a camera
   * Implementation is to be provided by OrtogonalCamera and PerspectiveCamera
   *
   */
  virtual void fire_ray() = 0;
  virtual void fire_all_rays() = 0;
};

/**
 * @brief A camera implementing an ortogonal 3D -> 2D projection
 * This class implements an observer seeing the world through an orthogonal
 * projection.
 *
 */

struct OrthogonalCamera : public Camera {
  Transformation transformation;
  float aspect_ratio;

  OrthogonalCamera(float _aspect_ratio = 1.f,
                   Transformation _transformation = Transformation())
      : aspect_ratio{_aspect_ratio}, transformation{_transformation} {}

  Ray fire_ray(float u, float v) {
    Point origin(-1.0, (1.0 - 2 * u) * aspect_ratio, 2 * v - 1.0);
    Vec dir = VEC_X;
    return Ray{origin, dir, 0, 1.0}.transform(transformation);
  }
};
