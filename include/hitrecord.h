#ifndef HITRECORD_H
#define HITRECORD_H
#include "ray.h"
/**
 * @brief A 2D vector used to represent a point on a surface
 *
 */
struct Vec2d {
  /**
   * @brief The class members are named u and v to distinguish them from x,y,z,
   * which are used for 3D vectors
   *
   */
  float u, v;
  /**
   * @brief Construct a new Vec 2d object
   *
   * @param _u
   * @param _v
   */
  Vec2d(float _u = 0.f, float _v = 0.f) : u{_u}, v{_v} {}

  /**
   * @brief Check whether two 2d vectors are the same
   *
   * @param other_vec2d
   * @return true
   * @return false
   */
  inline bool is_close(Vec2d other_vec2d) {
    return are_close(u, other_vec2d.u) && are_close(v, other_vec2d.v);
  }
};
/**
 * @brief A class representing an intersection between a ray and a shape
 *
 */
struct HitRecord {
  /**
   * @brief The members of this class are:
   * -`world_point`: a :class:`.Point` object holding the world coordinates of
   the hit point
     -`normal`: a :class:`.Normal` object holding the orientation of the normal
   to the surface where the hit happened
     -`surface_point`: a :class:`.Vec2d` object holding the position of the hit
   point on the surface of the object
     -`t`: a floating-point value specifying the distance from the origin of the
   ray where the hit happened
     -`ray`: the ray that hit the surface
     - `hit`: says whether the ray hit the surface
   *
   */
  Point world_point;
  Normal normal;
  Vec2d surface_point;
  Ray ray;
  float t;
  bool hit;

  /**
   * @brief Construct a new Hit Record object
   *
   * @param _world_point
   * @param _normal
   * @param _surface_point
   * @param _ray
   * @param _t
   * @param _hit
   */
  HitRecord(Point _world_point = Point(), Normal _normal = Normal(),
            Vec2d _surface_point = Vec2d(), Ray _ray = Ray(), float _t = 0.f,
            bool _hit = false)
      : world_point{_world_point}, normal{_normal},
        surface_point{_surface_point}, ray{_ray}, t{_t}, hit{_hit} {}

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