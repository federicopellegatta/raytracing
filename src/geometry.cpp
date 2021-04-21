#include "geometry.h"

////////////////////
/* TRANSFORMATION */
////////////////////

// Default constructor
// Initialize the identical transformation
Transformation::Transformation() {
  for (int i{}; i < 4; i++) {
    for (int j{}; j < 4; j++) {
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

// Constructor which takes two matrices as argument
// and initialize a transformation
Transformation::Transformation(float _m[4][4], float _invm[4][4]) {
  for (int i{}; i < 4; i++) {
    for (int j{}; j < 4; j++) {
      m[i][j] = _m[i][j];
      invm[i][j] = _invm[i][j];
    }
  }
}

void _matr_prod(const float a[4][4], const float b[4][4], float c[4][4]) {
  for (int i{}; i < 4; i++) {
    for (int j{}; j < 4; j++) {
      for (int k{}; k < 4; k++)
        c[i][j] += a[i][k] * b[k][j];
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

bool _are_matr_close(float a[4][4], float b[4][4]) {
  for (int i{}; i < 4; i++) {
    for (int j{}; j < 4; j++) {
      if (!(are_close(a[i][j], a[i][j])))
        return false;
      else
        return true;
    }
  }
}

// Mul operation overloading
Vec Transformation::operator*(Vec vec) {
  return Vec{vec.x * m[0][0] + vec.y * m[0][1] + vec.z * m[0][2],
             vec.x * m[1][0] + vec.y * m[1][1] + vec.z * m[1][2],
             vec.x * m[2][0] + vec.y * m[2][1] + vec.z * m[2][2]};
}
Point Transformation::operator*(Point p) {
  Point q{p.x * m[0][0] + p.y * m[0][1] + p.z * m[0][2],
          p.x * m[1][0] + p.y * m[1][1] + p.z * m[1][2],
          p.x * m[2][0] + p.y * m[2][1] + p.z * m[2][2]};
  float w = p.x * m[3][0] + p.y * m[3][1] + p.z * m[3][2] + m[3][3];

  if (w == 1)
    return q;
  else
    return Point(q.x / w, q.y / w, q.z / w);
}

Normal Transformation::operator*(Normal n) {
  return Normal{n.x * m[0][0] + n.y * m[0][1] + n.z * m[0][2],
                n.x * m[1][0] + n.y * m[1][1] + n.z * m[1][2],
                n.x * m[2][0] + n.y * m[2][1] + n.z * m[2][2]};
}

Transformation Transformation::operator*(Transformation t) {
  float result_m[4][4] = {};
  float result_invm[4][4] = {};
  _matr_prod(this->m, t.m, result_m);
  _matr_prod(t.invm, this->invm, result_invm); // Reverse order

  return Transformation{result_m, result_invm};
}
