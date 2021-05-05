#ifndef SHAPES_H
#define SHAPES_H

#include "hitrecord.h"
#include "ray.h"

/**
 * @brief Convert a 3D point on the surface of the unit sphere into a (u, "
              "v) 2D point"
 *
 * @param point A `Point` object
 * @return Vec2d
 *
 * @see Point
 */
Vec2d _sphere_point_to_uv(Point point) {
  return Vec2d(atan2(point.y, point.x) / (2.0 * M_PI), acos(point.z) / M_PI);
}

/**
 * @brief Compute the normal of a unit sphere
    The normal is computed for `point` (a point on the surface of the
    sphere), and it is chosen so that it is always in the opposite
    direction with respect to `ray_dir`.
 *
 * @param point A `Point` object on the surface of the sphere where to compute
 the normal
 * @param ray_dir A `Vec` which indicates the ray direction
 * @return Normal
 *
 * @see Point
 * @see Vec
 * @see Normal
 */
Normal _sphere_normal(Point point, Vec ray_dir) {
  Normal result = Normal(point.x, point.y, point.z);

  return point.to_vec().dot(ray_dir) < 0.0 ? result : -1 * result;
}

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

  Sphere(Transformation _transformation = Transformation())
      : transformation{_transformation} {}

  /**
   * @brief Checks if a ray intersects the sphere
   *
   * @param ray
   * @return Return a `HitRecord`, or `NULL` if no intersection was found.
   *
   * @see Ray
   * @see HitRecord
   */
  HitRecord ray_intersection(Ray ray) {
    Ray inv_ray = ray.transform(transformation.inverse());
    Vec origin_vec = inv_ray.origin.to_vec();
    bool hit = true;

    float a = inv_ray.dir.squared_norm();
    float b = 2.0 * origin_vec.dot(inv_ray.dir);
    float c = origin_vec.squared_norm() - 1.0;

    float delta = b * b - 4.0 * a * c;
    if (delta <= 0.0)
      hit = false;

    float tmin = (-b - sqrt(delta)) / (2.0 * a);
    float tmax = (-b + sqrt(delta)) / (2.0 * a);
    float first_hit_t;

    if (tmin > inv_ray.tmin && tmin < inv_ray.tmax)
      first_hit_t = tmin;
    else if (tmax > inv_ray.tmin && tmax < inv_ray.tmax)
      first_hit_t = tmax;
    else
      hit = false;

    Point hit_point = inv_ray.at(first_hit_t);

    return HitRecord(transformation * hit_point,
                     transformation * _sphere_normal(hit_point, ray.dir),
                     _sphere_point_to_uv(hit_point), first_hit_t, ray, hit);
  }
};

#endif