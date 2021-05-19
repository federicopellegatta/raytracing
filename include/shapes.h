#ifndef SHAPES_H
#define SHAPES_H

#include "hitrecord.h"
#include "materials.h"
#include "ray.h"

/** Shape class
 * @brief A generic 3D shape
    This is an abstract class, and you should only use it to
 derive concrete classes. Be sure to redefine the method
    :meth:`.Shape.ray_intersection`.
 *
 */
struct Shape {
  /**
   * @brief Compute the intersection between a ray and this shape
   *
   * @return HitRecord
   */
  virtual HitRecord ray_intersection(Ray) = 0;
};

/**
 * @brief 3D unit sphere centered on the origin of the axes
 *
 * @param _transformation
 */
struct Sphere : public Shape {
  Transformation transformation;
  Material material;

  Sphere(Transformation _transformation = Transformation(),
         Material _material = Material())
      : transformation{_transformation}, material{_material} {}

  /**
   * @brief Checks if a ray intersects the sphere
   *
   * @param ray
   * @return Return a `HitRecord`, or `NULL` if no intersection was found.
   *
   * @see Ray
   * @see HitRecord
   */
  HitRecord ray_intersection(Ray);

private:
  /**
 * @brief Convert a 3D point on the surface of the unit sphere into a (u, "
              "v) 2D point"
 *
 * @param point A `Point` object
 * @return Vec2d
 *
 * @see Point
 */
  inline Vec2d sphere_point_to_uv(Point point) {
    float u = atan2(point.y, point.x) / (float)(2.0 * M_PI);
    if (u < 0)
      u = u + 1;
    return Vec2d{u, acos(point.z) / (float)(M_PI)};
  }

  /**
   * @brief Compute the normal of a unit sphere
      The normal is computed for `point` (a point on the surface of the
      sphere), and it is chosen so that it is always in the opposite
      direction with respect to `ray_dir`.
   *
   * @param point A `Point` object on the surface of the sphere where to
   compute the normal
   * @param ray_dir A `Vec` which indicates the ray direction
   * @return Normal
   *
   * @see Point
   * @see Vec
   * @see Normal
   */
  inline Normal sphere_normal(Point point, Vec ray_dir) {
    Normal result{point.x, point.y, point.z};
    return point.to_vec().dot(ray_dir) < 0.0 ? result : -1 * result;
  }
};

/**
 * @brief A plane in 3D space. The default plane is the x-y
 *
 * @param _transformation
 */
struct Plane : public Shape {
  Transformation transformation;
  Material material;

  Plane(Transformation _transformation = Transformation(),
        Material _material = Material())
      : transformation{_transformation}, material{_material} {}

  /**
   * @brief Checks if a ray intersects the sphere
   *
   * @param ray
   * @return Return a `HitRecord`, or `NULL` if no intersection was found.
   *
   * @see Ray
   * @see HitRecord
   */
  HitRecord ray_intersection(Ray);

private:
  /**
   * @brief Convert a 3D point on the surface of the plane into a (u, v) 2D
   * point"
   *
   * @param point A `Point` object
   * @return Vec2d
   *
   * @see Point
   */
  inline Vec2d plane_point_to_uv(Point point) {
    return Vec2d{point.x - floor(point.x), point.y - floor(point.y)};
  }

  /**
   * @brief Compute the normal of a plane
      The normal is computed for `point` (a point on the surface of the
      plane), and it is chosen so that it is always in the opposite
      direction with respect to `ray_dir`.
   *
   * @param point A `Point` object on the surface of the plane where to
   compute the normal
   * @param ray_dir A `Vec` which indicates the ray direction
   * @return Normal
   *
   * @see Point
   * @see Vec
   * @see Normal
   */
  inline Normal plane_normal(Point point, Vec ray_dir) {
    Normal result{0.0, 0.0, 1.0};
    return ray_dir.z < 0.0 ? result : -1 * result;
  }
};
#endif