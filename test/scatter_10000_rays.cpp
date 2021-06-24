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

#include "geometry.h"
#include "materials.h"
#include "ray.h"
#include <fstream>
#include <memory>

using namespace std;

int main() {

  DiffusiveBRDF scatter_10000_rays;
  ofstream rays_coordinates("rays_coordinates.txt");
  PCG pcg;
  if (rays_coordinates.is_open()) {
    for (int i{}; i < 100000; ++i) {
      Ray ray = scatter_10000_rays.scatter_ray(pcg, Vec(), Point(),
                                               VEC_Z.to_norm(), 2);
      // fmt::print("x = {}, y = {}, z = {}\n", ray.origin.x + ray.dir.x,
      //           ray.origin.y + ray.dir.y, ray.origin.z + ray.dir.z);
      rays_coordinates << ray.origin.x + ray.dir.x << " "
                       << ray.origin.y + ray.dir.y << " "
                       << ray.origin.z + ray.dir.z << endl;
    }
  } else {
    fmt::print("ERROR: can't open output file.\nExiting.\n");
  }
  rays_coordinates.close();

  return 0;
}