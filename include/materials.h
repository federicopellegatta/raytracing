#ifndef MATERIALS_H
#define MATERIALS_H

#include "HdrImage.h"
#include "colors.h"
#include "geometry.h"

struct Pigment {
  virtual Color get_color(Vec2d) = 0;
};

struct UniformPigment : public Pigment {
  Color color;
  UniformPigment(Color _color = Color()) : color{_color} {}
  Color get_color(Vec2d uv) { return color; }
};

struct CheckeredPigment : public Pigment {
  Color color1, color2;
  int num_of_steps;

  CheckeredPigment(Color _color1 = Color(), Color _color2 = Color(),
                   int _num_of_steps = 10)
      : color1{_color1}, color2{_color2} {
    num_of_steps = _num_of_steps;
  }
  Color get_color(Vec2d uv);
};

struct ImagePigment : public Pigment {
  HdrImage image;
  ImagePigment(HdrImage _image) : image{_image} {};
  Color get_color(Vec2d uv);
};

struct BRDF {
  shared_ptr<Pigment> pigment;

  BRDF(shared_ptr<Pigment> _pigment = make_shared<UniformPigment>(WHITE))
      : pigment{_pigment} {}

  inline virtual Color eval(Normal normal, Vec inc_dir, Vec out_dir,
                            Vec2d uv) = 0;
};

struct DiffusiveBRDF : public BRDF {
  float reflectance;

  DiffusiveBRDF(
      shared_ptr<Pigment> _pigment = make_shared<UniformPigment>(WHITE),
      float _reflectance = 1.)
      : BRDF{_pigment} {
    reflectance = _reflectance;
  }

  inline Color eval(Normal normal, Vec inc_dir, Vec out_dir, Vec2d uv) {
    return pigment->get_color(uv) * (reflectance / M_PI);
  }
};

struct Material {
  shared_ptr<BRDF> brdf;
  shared_ptr<Pigment> emitted_radiance;

  Material(shared_ptr<BRDF> _brdf = make_shared<DiffusiveBRDF>(),
           shared_ptr<Pigment> _emitted_radiance =
               make_shared<UniformPigment>(BLACK))
      : brdf{_brdf}, emitted_radiance{_emitted_radiance} {}
};
#endif