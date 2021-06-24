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
#include "world.h"
#include <cassert>

using namespace std;

void test_sphere_hit() {
  Sphere sphere;
  Ray ray1(Point(0., 0., 2.), -VEC_Z);

  HitRecord intersection1 = sphere.ray_intersection(ray1);
  assert(intersection1.hit);
  assert(HitRecord(make_shared<Sphere>(sphere), Point(0., 0., 1.),
                   Normal(0., 0., 1.), Vec2d(0., 0.), 1.0, ray1, false)
             .is_close(intersection1));

  Ray ray2 = Ray(Point(3, 0, 0), -VEC_X);
  HitRecord intersection2 = sphere.ray_intersection(ray2);
  assert(intersection2.hit);
  assert(HitRecord(make_shared<Sphere>(sphere), Point(1.0, 0.0, 0.0),
                   Normal(1.0, 0.0, 0.0), Vec2d(0.0, 0.5), 2.0, ray2, false)
             .is_close(intersection2));

  assert(!sphere.ray_intersection(Ray(Point(0, 10, 2), -VEC_Z)).hit);
}

void test_sphere_inner_hit() {
  Sphere sphere;

  Ray ray(Point(0, 0, 0), VEC_X);
  HitRecord intersection = sphere.ray_intersection(ray);
  assert(intersection.hit);
  assert(HitRecord(make_shared<Sphere>(sphere), Point(1.0, 0.0, 0.0),
                   Normal(-1.0, 0.0, 0.0), Vec2d(0.0, 0.5), 1.0, ray, false)
             .is_close(intersection));
}

void test_sphere_transformation() {
  Sphere sphere{translation(Vec(10.0, 0.0, 0.0))};

  Ray ray1{Point(10, 0, 2), -VEC_Z};
  HitRecord intersection1 = sphere.ray_intersection(ray1);
  assert(intersection1.hit);
  assert(HitRecord(make_shared<Sphere>(sphere), Point(10.0, 0.0, 1.0),
                   Normal(0.0, 0.0, 1.0), Vec2d(0.0, 0.0), 1.0, ray1, false)
             .is_close(intersection1));

  Ray ray2{Point(13, 0, 0), -VEC_X};
  HitRecord intersection2 = sphere.ray_intersection(ray2);
  assert(intersection2.hit);
  assert(HitRecord(make_shared<Sphere>(sphere), Point(11.0, 0.0, 0.0),
                   Normal(1.0, 0.0, 0.0), Vec2d(0.0, 0.5), 2.0, ray2, false)
             .is_close(intersection2));

  // Check if the sphere failed to move by trying to hit the untransformed shape
  assert(!sphere.ray_intersection(Ray(Point(0, 0, 2), -VEC_Z)).hit);

  // Check if the *inverse *transformation was wrongly applied
  assert(!sphere.ray_intersection(Ray(Point(-10, 0, 0), -VEC_Z)).hit);
}

void test_sphere_normal() {
  Sphere sphere{scaling(Vec(2.0, 1.0, 1.0))};

  Ray ray{Point(1.0, 1.0, 0.0), Vec(-1.0, -1.0)};
  HitRecord intersection = sphere.ray_intersection(ray);

  // We normalize "intersection.normal", as we are not interested in its length
  intersection.normal.normalize();
  Normal n(1., 4., 0.);
  n.normalize();
  assert(intersection.normal == n);
}

void test_sphere_normal_direction() {

  Sphere sphere;
  Ray ray{Point(0.0, 0.0, 0.0), VEC_Y};
  HitRecord intersection = sphere.ray_intersection(ray);

  // We normalize "intersection.normal", as we are not interested in its length
  intersection.normal.normalize();
  assert(intersection.normal.to_vec() == -VEC_Y);
}

void test_sphere_uv_coordinates() {
  Sphere sphere{};

  Ray ray1{Point(2.0, 0.0, 0.0), -VEC_X};
  assert(sphere.ray_intersection(ray1).surface_point.is_close(Vec2d(0.0, 0.5)));

  Ray ray2{Point(0.0, 2.0, 0.0), -VEC_Y};
  assert(
      sphere.ray_intersection(ray2).surface_point.is_close(Vec2d(0.25, 0.5)));

  Ray ray3{Point(-2.0, 0.0, 0.0), VEC_X};
  assert(sphere.ray_intersection(ray3).surface_point.is_close(Vec2d(0.5, 0.5)));

  Ray ray4 = {Point(0.0, -2.0, 0.0), VEC_Y};
  assert(
      sphere.ray_intersection(ray4).surface_point.is_close(Vec2d(0.75, 0.5)));

  Ray ray5{Point(2.0, 0.0, 0.5), -VEC_X};
  assert(sphere.ray_intersection(ray5).surface_point.is_close(
      Vec2d(0.0, 1. / 3.)));

  Ray ray6{Point(2.0, 0.0, -0.5), -VEC_X};
  assert(sphere.ray_intersection(ray6).surface_point.is_close(
      Vec2d(0.0, 2. / 3.)));
}

// Plane test
void test_plane_hit() {
  Plane plane;
  Ray ray1(Point(0., 0., 2.), -VEC_Z);

  HitRecord intersection1 = plane.ray_intersection(ray1);
  assert(intersection1.hit);
  assert(HitRecord(make_shared<Plane>(plane), Point(0., 0., 0.),
                   Normal(0., 0., 1.), Vec2d(0., 0.), 2.0, ray1, true)
             .is_close(intersection1));

  Ray ray2(Point(0., 0., 2.), VEC_X);
  HitRecord intersection2 = plane.ray_intersection(ray2);
  assert(!intersection2.hit);
}

void test_plane_transformation() {
  Plane plane1(rotation_x(90));
  Ray ray1(Point(0., 2., 0.), -VEC_Y);
  HitRecord intersection1{plane1.ray_intersection(ray1)};
  assert(intersection1.hit);
  assert(HitRecord(make_shared<Plane>(plane1), Point(0., 0., 0.),
                   Normal(0., 1., 0.), Vec2d(0., 0.), 2.0, ray1, true)
             .is_close(intersection1));

  Ray ray2(Point(0., 1., 0.), VEC_X);
  HitRecord intersection2{plane1.ray_intersection(ray2)};
  assert(!intersection2.hit);

  Plane plane2{translation(VEC_Z)};
  Ray ray3(Point(0., 0., 2.), -VEC_Z);

  HitRecord intersection3 = plane2.ray_intersection(ray3);
  assert(intersection3.hit);
  assert(HitRecord(make_shared<Plane>(plane2), Point(0., 0., 1.),
                   Normal(0., 0., 1.), Vec2d(0., 0.), 1.0, ray3, true)
             .is_close(intersection3));
}

// World test
void test_ray_intersection() {
  World world;

  Sphere sphere1{translation(VEC_X * 2)};
  Sphere sphere2{translation(VEC_X * 8)};
  world.add(make_shared<Sphere>(sphere1));
  world.add(make_shared<Sphere>(sphere2));

  HitRecord intersection1 =
      world.ray_intersection(Ray(Point(0.0, 0.0, 0.0), VEC_X));
  HitRecord intersection2 =
      world.ray_intersection(Ray(Point(10.0, 0.0, 0.0), -VEC_X));

  assert(intersection1.hit);
  assert(intersection1.world_point == (Point{1.f, 0.f, 0.f}));

  assert(intersection2.hit);
  assert(intersection2.world_point == (Point{9.f, 0.f, 0.f}));
}

int main() {

  // Sphere tests
  test_sphere_hit();
  test_sphere_inner_hit();
  test_sphere_transformation();
  test_sphere_normal();
  test_sphere_normal_direction();
  test_sphere_uv_coordinates();

  // Plane tests
  test_plane_hit();
  test_plane_transformation();

  // World tests
  test_ray_intersection();
  return 0;
}