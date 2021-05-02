#include "geometry.h"
#include <iostream>
#include <limits>

using namespace std;

/**
 * @brief A ray of light propagating in space
 * The class contains the following members:
    -   `origin` (``Point``): the 3D point where the ray originated
    -   `dir` (``Vec``): the 3D direction along which this ray propagates
    -   `tmin` (float): the minimum distance travelled by the ray is this number
 times `dir`
    -   `tmax` (float): the maximum distance travelled by the ray is this number
 times `dir`
    -   `depth` (int): number of times this ray was reflected/refracted"""

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
   * @return Point The point in 3D space whose distance from the
        ray's origin is equal to `t`
   */
  inline Point at(float t) { return origin + dir * t; }

  /**
   * @brief Transform a ray
    This method returns a new ray whose origin and direction are the
   transformation of the original ray
   *
   * @param transformation
   * @return Ray
   */
  Ray transform(Transformation transformation) {
    return Ray(transformation * origin, transformation * dir, tmin, tmax,
               depth);
  }
};