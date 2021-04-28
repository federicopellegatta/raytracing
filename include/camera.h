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

struct OrtogonalCamera : public Camera {
  Transformation transformation;
  float aspect_ratio;

  OrtogonalCamera(float, Transformation);

  Ray fire_ray(float, float);
};
