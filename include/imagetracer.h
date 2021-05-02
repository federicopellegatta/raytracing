#include "HdrImage.h"
#include "camera.h"

using namespace std;

/**
 * @brief Trace an image by shooting light rays through each of its pixels
 *
 */
struct ImageTracer {
  HdrImage image;
  Camera camera;

  ImageTracer(HdrImage _image, OrthogonalCamera _camera)
      : image{_image}, camera{_camera} {}
  ImageTracer(HdrImage _image, PerspectiveCamera _camera)
      : image{_image}, camera{_camera} {}

  /**
   * @brief Shoot one light ray through image pixel (col, row)
   *
   * The parameters (col, row) are measured in the same way as they are in
   :class:`.HdrImage`: the bottom left corner is placed at (0, 0).
   The values of `u_pixel` and `v_pixel` are floating-point numbers in the range
   [0, 1]. They specify where the ray should cross the pixel; passing 0.5 to
   both means that the ray will pass through the pixel's center.
   *
   * @param col image column
   * @param row image row
   * @param u_pixel `u` coordinate in pixel frame
   * @param v_pixel `v` coordinate in pixel frame
   * @return Ray the Ray passing through (col, row) starting from the observer
   */
  Ray fire_ray(int col, int row, float u_pixel = 0.5, float v_pixel = 0.5) {
    float u = (col + u_pixel) / (image.width - 1);
    float v = (row + v_pixel) / (image.height - 1);

    return camera.fire_ray(u, v);
  }

  /**
   * @brief Shoot several light rays crossing each of the pixels in the image
   *
   * For each pixel in the :class:`.HdrImage` object fire one ray, and pass it
   to the function `func`
   *
   * @param func It must accept :class:`.Ray` as its only parameter and must
   return a :class:`.Color` instance telling the color to
   assign to that pixel in the image.
   */
  template <typename F> void fire_all_rays(F func) {
    for (int row{}; row < image.height; row++) {
      for (int col{}; col < image.width; col++) {
        Ray ray = fire_ray(col, row);
        Color color = func(ray);
        image.set_pixel(col, row, color);
      }
    }
  }
};
