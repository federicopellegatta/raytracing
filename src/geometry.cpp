#include "geometry.h"

Transformation::Transformation(float **_m, float **_invm) {
  for (int i{}; i < 4; i++) {
    for (int j{}; j < 4; i++) {
      m[i][j] = _m[i][j];
      invm[i][j] = _invm[i][j];
    }
  }
}

Transformation::Transformation() {
  for (int i{}; i < 4; i++) {
    for (int j{}; j < 4; i++) {
      if (i == j) {
        m[i][j] = 1;
        invm[i][j] = 1;
      } else {
        m[i][j] = 0;
        invm[i][j] = 0;
      }
    }
  }
}

Transformation translation(Vec vec) {
  float _m[4][4] = {{1.0, 0.0, 0.0, vec.x},
                    {0.0, 1.0, 0.0, vec.y},
                    {0.0, 0.0, 1.0, vec.z},
                    {0.0, 0.0, 0.0, 1.0}};
  float _invm[4][4] = {{1.0, 0.0, 0.0, -vec.x},
                       {0.0, 1.0, 0.0, -vec.y},
                       {0.0, 0.0, 1.0, -vec.z},
                       {0.0, 0.0, 0.0, 1.0}};

  return Transformation(_m, _invm);
}