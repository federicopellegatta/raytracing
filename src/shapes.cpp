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

#include "shapes.h"

HitRecord Sphere::ray_intersection(Ray ray) {
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
  float first_hit_t = 0.f;

  if (tmin > inv_ray.tmin && tmin < inv_ray.tmax)
    first_hit_t = tmin;
  else if (tmax > inv_ray.tmin && tmax < inv_ray.tmax)
    first_hit_t = tmax;
  else
    hit = false;

  Point hit_point = inv_ray.at(first_hit_t);

  return HitRecord(make_shared<Sphere>(*this), transformation * hit_point,
                   transformation * sphere_normal(hit_point, ray.dir),
                   sphere_point_to_uv(hit_point), first_hit_t, ray, hit);
}

HitRecord Plane::ray_intersection(Ray ray) {
  Ray inv_ray = ray.transform(transformation.inverse());
  Vec origin_vec = inv_ray.origin.to_vec();
  bool hit = true;

  if (are_close(inv_ray.dir.z, 0.0)) // if ray is parallel to the plane
    hit = false;

  float t = -origin_vec.z / inv_ray.dir.z;

  if (t <= inv_ray.tmin || t >= inv_ray.tmax)
    hit = false;

  Point hit_point = inv_ray.at(t);
  return HitRecord(make_shared<Plane>(*this), transformation * hit_point,
                   transformation * plane_normal(hit_point, ray.dir),
                   plane_point_to_uv(hit_point), t, ray, hit);
}
