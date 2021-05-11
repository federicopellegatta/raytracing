#ifndef RAY_H
#define RAY_H
#include "geometry.h"
#include <iostream>
#include <limits>

using namespace std;

/** Ray class
 * @brief A ray of light propagating in space
 *
 * @param origin A `Point` where the ray is originated
 * @param dir A `Vec`that indicates the direction along which the ray propagates
 * @param depth number of times this ray was reflected/refracted
 * @param tmin The minimum distance travelled by the ray is this float number
 * @param tmax The maximum distance travelled by the ray is this float number
 *
 * @see Point
 * @see Vec
 */
struct Ray {
  Point origin = Point();
  Vec dir = Vec();
  float tmin = 1e-5;
  float tmax = numeric_limits<float>::infinity();
  int depth = 0;

  Ray(Point _origin = Point(), Vec _dir = Vec(), int _depth = 0,
      float _tmin = 1e-5, float _tmax = std::numeric_limits<float>::infinity())
      : origin{_origin}, dir{_dir}, depth{_depth}, tmin{_tmin}, tmax{_tmax} {}

  bool operator==(Ray other) {
    return (origin == other.origin) && (dir == other.dir);
  }
  bool operator!=(Ray other) { return !(*this == other); }

  /**
   * @brief Compute the point along the ray's path at some distance from the
   * origin
   *
   * @param t Length in `dir` unit
   * @return The point in 3D space whose distance from the
        ray's origin is equal to `t`
   */
  inline Point at(float t) { return origin + dir * t; }

  /**
   * @brief Transform a ray
   * This method returns a new ray whose origin and direction are the
   transformation of the original ray
   *
   * @param transformation
   * @return The ray transformed
   */
  inline Ray transform(Transformation transformation) {
    return Ray(transformation * origin, transformation * dir, depth, tmin,
               tmax);
  }
};
#endif