#include "geometry.h"

float IDENTITY_MATR4x4[4][4] = {{1.f, 0.f, 0.f, 0.f},
                                {0.f, 1.f, 0.f, 0.f},
                                {0.f, 0.f, 1.f, 0.f},
                                {0.f, 0.f, 0.f, 1.f}};

////////////////////
/* TRANSFORMATION */
////////////////////

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

// Comparison between two matrices
bool _are_matr_close(float a[4][4], float b[4][4]) {
  for (int i{}; i < 4; i++) {
    for (int j{}; j < 4; j++) {
      if (!(are_close(a[i][j], a[i][j])))
        return false;
    }
  }
  return true;
}

// Product between two 4x4 matrices
void _matr_prod(const float a[4][4], const float b[4][4], float c[4][4]) {
  for (int i{}; i < 4; i++) {
    for (int j{}; j < 4; j++) {
      for (int k{}; k < 4; k++)
        c[i][j] += a[i][k] * b[k][j];
    }
  }
}

bool Transformation::is_close(Transformation t) {
  return _are_matr_close(m, t.m) and _are_matr_close(invm, t.invm);
}

bool Transformation::is_consistent() {
  float prod[4][4] = {};
  _matr_prod(m, invm, prod);
  return _are_matr_close(prod, IDENTITY_MATR4x4);
}

string Transformation::to_str() {
  ostringstream stream;
  stream << "[\n";
  for (int i{}; i < 4; i++) {
    for (int j{}; j < 4; j++) {
      stream << m[i][j] << " ";
    }
    stream << "\n";
  }
  stream << " ]\n";
  return stream.str();
}

Transformation Transformation::inverse() { return Transformation{invm, m}; }

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
  return Normal{n.x * invm[0][0] + n.y * invm[1][0] + n.z * invm[2][0],
                n.x * invm[0][1] + n.y * invm[1][1] + n.z * invm[2][1],
                n.x * invm[0][2] + n.y * invm[1][2] + n.z * invm[2][2]};
}

Transformation Transformation::operator*(Transformation t) {
  float result_m[4][4] = {};
  float result_invm[4][4] = {};
  _matr_prod(this->m, t.m, result_m);
  _matr_prod(t.invm, this->invm, result_invm); // Reverse order

  return Transformation{result_m, result_invm};
}

// Translation transformation
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

Transformation scaling(Vec vec) {
  float _m[4][4] = {{vec.x, 0.0, 0.0, 0.0},
                    {0.0, vec.y, 0.0, 0.0},
                    {0.0, 0.0, vec.z, 0.0},
                    {0.0, 0.0, 0.0, 1.0}};
  float _invm[4][4] = {{1 / vec.x, 0.0, 0.0, 0.0},
                       {0.0, 1 / vec.y, 0.0, 0.0},
                       {0.0, 0.0, 1 / vec.z, 0.0},
                       {0.0, 0.0, 0.0, 1.0}};

  return Transformation(_m, _invm);
}

Transformation rotation_x(float ang) { // angolo in rad
  // float ang = ang * PI / 180;
  float cosang = cos(ang);
  float sinang = sin(ang);

  float _m[4][4] = {{1.0, 0.0, 0.0, 0.0},
                    {0.0, cosang, -sinang, 0.0},
                    {0.0, sinang, cosang, 0.0},
                    {0.0, 0.0, 0.0, 1.0}};
  float _invm[4][4] = {{1.0, 0.0, 0.0, 0.0},
                       {0.0, cosang, sinang, 0.0},
                       {0.0, -sinang, cosang, 0.0},
                       {0.0, 0.0, 0.0, 1.0}};

  return Transformation(_m, _invm);
}

Transformation rotation_y(float ang) { // angolo in rad
  // float ang = ang * PI / 180;
  float cosang = cos(ang);
  float sinang = sin(ang);

  float _m[4][4] = {{cosang, 0.0, sinang, 0.0},
                    {0.0, 1.0, 0.0, 0.0},
                    {-sinang, 0.0, cosang, 0.0},
                    {0.0, 0.0, 0.0, 1.0}};
  float _invm[4][4] = {{cosang, 0.0, -sinang, 0.0},
                       {0.0, 1.0, 0.0, 0.0},
                       {sinang, 0.0, cosang, 0.0},
                       {0.0, 0.0, 0.0, 1.0}};

  return Transformation(_m, _invm);
}

Transformation rotation_z(float ang) { // angolo in rad
  // float ang = ang * PI / 180;
  float cosang = cos(ang);
  float sinang = sin(ang);

  float _m[4][4] = {{cosang, -sinang, 0.0, 0.0},
                    {sinang, cosang, 0.0, 0.0},
                    {0.0, 0.0, 1.0, 0.0},
                    {0.0, 0.0, 0.0, 1.0}};
  float _invm[4][4] = {{cosang, sinang, 0.0, 0.0},
                       {-sinang, cosang, 0.0, 0.0},
                       {0.0, 0.0, 1.0, 0.0},
                       {0.0, 0.0, 0.0, 1.0}};

  return Transformation(_m, _invm);
}
