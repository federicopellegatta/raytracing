#ifndef MATERIALS_H
#define MATERIALS_H

#include "HdrImage.h"
#include "colors.h"
#include "geometry.h"

/**
 * @brief A pigment: this abstract class represents a pigment, i.e., a function
 that associates a color with each point on a parametric surface (u,v). Call the
 method `get_color` to retrieve the color of the surface given a
 `Vec2d` object.
 *
 */
struct Pigment {
  /**
   * @brief Get the color object at the specified coordinates. This is a pure
   * virtual function, it is to be re-implemented in the derived classes.
   *
   * @return Color
   */
  virtual Color operator()(Vec2d) = 0;
};

/**
 * @brief An uniform pigment: an uniform hue over the whole surface
 *
 */
struct UniformPigment : public Pigment {
  Color color;
  UniformPigment(Color _color = Color()) : color{_color} {}
  Color operator()(Vec2d uv) { return color; }
};

/**
 * @brief A checkered pigment: two colors that repeats themselves each
 `num_of_steps`,
  The number of rows/columns in the checkered pattern is tunable, but you
 cannot have a different number of repetitions along the u/v directions.
 *
 */
struct CheckeredPigment : public Pigment {
  Color color1, color2;
  int num_of_steps;

  CheckeredPigment(Color _color1 = Color(), Color _color2 = Color(),
                   int _num_of_steps = 10)
      : color1{_color1}, color2{_color2} {
    num_of_steps = _num_of_steps;
  }
  Color operator()(Vec2d uv);
};

/**
 * @brief A pigment which texture is given by a PFM image file.
 *
 */
struct ImagePigment : public Pigment {
  HdrImage image;
  ImagePigment(HdrImage _image) : image{_image} {};
  Color operator()(Vec2d uv);
};

/**
 * @brief An abstract class representing Bidirectional Reflectance Distribution
 * Function
 *
 */
struct BRDF {
  shared_ptr<Pigment> pigment;

  BRDF(shared_ptr<Pigment> _pigment = make_shared<UniformPigment>(WHITE))
      : pigment{_pigment} {}

  /**
   * @brief Evaluate the color on the surface a uv coordinates. This is a pure
   * virtual method, it will be implemented in the derived classes
   *
   * @param normal
   * @param inc_dir
   * @param out_dir
   * @param uv
   * @return Color
   */
  inline virtual Color eval(Normal normal, Vec inc_dir, Vec out_dir,
                            Vec2d uv) = 0;
};

/**
 * @brief A class representing an ideal diffuse BRDF (also called «Lambertian»)
 *
 */
struct DiffusiveBRDF : public BRDF {
  float reflectance;

  DiffusiveBRDF(
      shared_ptr<Pigment> _pigment = make_shared<UniformPigment>(WHITE),
      float _reflectance = 1.)
      : BRDF{_pigment} {
    reflectance = _reflectance;
  }

  inline Color eval(Normal normal, Vec inc_dir, Vec out_dir, Vec2d uv) {
    return (*pigment)(uv) * (reflectance / M_PI);
  }
};

/**
 * @brief A material, which is non other that a BRDF and a pigment
 *
 */
struct Material {
  shared_ptr<BRDF> brdf;
  shared_ptr<Pigment> emitted_radiance;

  Material(shared_ptr<BRDF> _brdf = make_shared<DiffusiveBRDF>(),
           shared_ptr<Pigment> _emitted_radiance =
               make_shared<UniformPigment>(BLACK))
      : brdf{_brdf}, emitted_radiance{_emitted_radiance} {}
};
#endif