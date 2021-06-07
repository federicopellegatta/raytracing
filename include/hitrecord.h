#ifndef HITRECORD_H
#define HITRECORD_H

#include "geometry.h"
#include "ray.h"

/**
 * @brief "Redefining" class Shape in order to avoid circular references
 * This works because we only use a pointer to Shape
 */
class Shape;

/**
 * @brief A class representing an intersection between a ray and a shape
 *
 * @param world_point a :class:`.Point` object holding the world coordinates of
 the hit point
 * @param normal a :class:`.Normal` object holding the orientation of the normal
 to the surface where the hit happened
 * @param surface_point a :class:`.Vec2d` object holding the position of the hit
   point on the surface of the object
 * @param t a floating-point value specifying the distance from the origin of
 the ray where the hit happened
 * @param ray the ray that hit the surface
 * @param hit a bool which says whether the ray hit the surface
 * @param shape pointer to the shape hit by the ray
 *
 * @see Point
 * @see Normal
 * @see Vec2d
 * @see Ray
 * @see Shape
 */
struct HitRecord {

  Point world_point;
  Normal normal;
  Vec2d surface_point;
  Ray ray;
  shared_ptr<Shape> shape;
  float t;
  bool hit;
  /**
   * @brief Construct a new Hit Record object
   *
   * @param world_point
   * @param normal
   * @param surface_point
   * @param t
   * @param ray
   * @param hit
   * @param shape
   */
  HitRecord(shared_ptr<Shape> _shape, Point _world_point = Point(),
            Normal _normal = Normal(), Vec2d _surface_point = Vec2d(),
            float _t = 0.f, Ray _ray = Ray(), bool _hit = false)
      : shape{_shape}, world_point{_world_point}, normal{_normal},
        surface_point{_surface_point}, ray{_ray} {
    t = _t;
    hit = _hit;
  }

  /**
   * @brief Check if two HitRecords are the same hit event or not
   *
   * @param other_hr
   * @return true
   * @return false
   */
  inline bool is_close(HitRecord other_hr) {
    return (world_point == other_hr.world_point) &&
           (normal == other_hr.normal) &&
           (surface_point.is_close(other_hr.surface_point)) &&
           (ray == other_hr.ray) && are_close(t, other_hr.t);
  }
};

#endif