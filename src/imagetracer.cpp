#include "imagetracer.h"

void ImageTracer::fire_all_rays(function<Color(const Ray &)> func) {
  for (int row{}; row < image.height; row++) {
    for (int col{}; col < image.width; col++) {
      Ray ray = fire_ray(col, row);
      Color color = func(ray);
      image.set_pixel(col, row, color);
    }
  }
}