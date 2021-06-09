#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "colors.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

////////////////////////////
/* VECS, POINT AND NORMAL */
////////////////////////////
struct Normal;

// Template functions
/**
 * @brief Template function that converts types defined in this header to string
 *
 * @tparam In
 * @param a
 * @return string
 */
template <typename In> string _to_string(const In &a) {
  return string{"(" + to_string(a.x) + ", " + to_string(a.y) + ", " +
                to_string(a.z) + ")"};
}

/**
 * @brief Template function to compare types defined in this header
 *
 * @tparam In
 * @param a
 * @param b
 * @return true
 * @return false
 */
template <typename In> bool _are_close(const In &a, const In &b) {
  return are_close(a.x, b.x) && are_close(a.y, b.y) && are_close(a.z, b.z);
}

/**
 * @brief Wrapper around _are_close.
 *
 * @tparam In
 * @param a
 * @param b
 * @return true
 * @return false
 */
template <typename In> bool operator==(const In &a, const In &b) {
  return _are_close<In>(a, b);
}
template <typename In> bool operator!=(const In &a, const In &b) {
  return !(_are_close<In>(a, b));
}

/**
 * @brief Template function to sum types defined in this header
 *
 * @tparam In1
 * @tparam In2
 * @tparam Out
 * @param a
 * @param b
 * @return Out
 */
template <typename In1, typename In2, typename Out>
Out _sum(const In1 &a, const In2 &b) {
  return Out{a.x + b.x, a.y + b.y, a.z + b.z};
}

/**
 * @brief Template function to multiply types defined in this header for a float
 *
 * @tparam In1
 * @tparam Out
 * @param a
 * @param b
 * @return Out
 */
template <typename In1, typename Out> Out _mul(const float &a, const In1 &b) {
  return Out{a * b.x, a * b.y, a * b.z};
}

/** Vec class
 * @brief A class which represent a 3D vector
 */
struct Vec {
  float x, y, z;

  /**
   * @brief Construct a new Vec object
   *
   * @param _x
   * @param _y
   * @param _z
   */
  Vec(float _x = 0, float _y = 0, float _z = 0) {
    x = _x;
    y = _y;
    z = _z;
  }

  /**
   * @brief Scalar product
   *
   * @param a
   * @return float
   */
  inline float dot(const Vec &a) { return a.x * x + a.y * y + a.z * z; }
  /**
   * @brief Vector product
   *
   * @param b
   * @return Vec
   */
  inline Vec cross(const Vec &b) {
    return Vec(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
  }

  /**
   * @brief Calculate the squared norm of a vector
   *
   * @return float
   */
  inline float squared_norm() { return pow(x, 2) + pow(y, 2) + pow(z, 2); }
  /**
   * @brief Calculate the norm of a vector
   *
   * @return float
   */
  inline float norm() { return sqrt(squared_norm()); }
  /**
   * @brief Normalize the Vec
   *
   * @return Vec
   */
  inline void normalize() {
    float norm = this->norm();
    this->x /= norm;
    this->y /= norm;
    this->z /= norm;
  }
  /**
   * @brief Convert a Vec to a string
   *
   * @return string
   */
  inline string to_str() { return string{"Vec" + _to_string(*this)}; }

  /**
   * @brief Converts a Vec to Normal
   *
   * @return Normal
   */
  Normal to_norm();

  /**
   * @brief Returns a Vec with inverted components
   *
   * @return Vec
   */
  Vec operator-() const { return Vec{-x, -y, -z}; }
};

/** Point class
 * @brief A class which represent a 3D point
 */
struct Point {
  float x, y, z;

  /**
   * @brief Construct a new Point object
   *
   * @param _x
   * @param _y
   * @param _z
   */
  Point(float _x = 0, float _y = 0, float _z = 0) {
    x = _x;
    y = _y;
    z = _z;
  }
  /**
   * @brief Convert to string a Point object
   *
   * @return string
   */
  inline string to_str() { return string{"Point" + _to_string(*this)}; }
  /**
   * @brief Convert to Vec a Point object
   *
   * @return Vec
   */
  inline Vec to_vec() { return Vec(x, y, z); }
};

/** Normal class
 * @brief A class which represent a 3D normal to a surface
 */
struct Normal {
  float x, y, z;

  /**
   * @brief Construct a new Normal object
   *
   * @param _x
   * @param _y
   * @param _z
   */
  Normal(float _x = 0, float _y = 0, float _z = 0) {
    x = _x;
    y = _y;
    z = _z;
  }
  /**
   * @brief Convert to string a Normal object
   *
   * @return string
   */
  inline string to_str() { return string{"Normal" + _to_string(*this)}; }

  /**
   * @brief Calculate the squared norm of a Normal
   *
   * @return float
   */
  inline float squared_norm() { return x * x + y * y + z * z; }

  /**
   * @brief Calculate the norm of a Normal
   *
   * @return float
   */
  inline float norm() { return sqrt(squared_norm()); }
  /**
   * @brief Normalize a Normal object
   *
   */
  inline void normalize() {
    float norm = this->norm();
    this->x /= norm;
    this->y /= norm;
    this->z /= norm;
  }
  /**
   * @brief Converts to Vec
   *
   * @return Vec
   */
  inline Vec to_vec() { return Vec(x, y, z); }
  /**
   * @brief Returns a Normal object with inverted components
   *
   * @return Normal
   */
  Normal operator-() const { return Normal{-x, -y, -z}; }
};

extern Vec VEC_X;
extern Vec VEC_Y;
extern Vec VEC_Z;

/**
 * @brief Sum operation between Vecs and Points
 *
 * @param a
 * @param b
 * @return Vec
 */
inline Vec operator+(const Vec &a, const Vec &b) {
  return _sum<Vec, Vec, Vec>(a, b);
}
inline Point operator+(const Point &a, const Vec &b) {
  return _sum<Point, Vec, Point>(a, b);
}

/**
 * @brief Mul operation between Vecs/Points and a floating point (and viceversa)
 *
 * @param a
 * @param b
 * @return Vec
 */
inline Vec operator*(const float &a, const Vec &b) {
  return _mul<Vec, Vec>(a, b);
}
inline Vec operator*(const Vec &a, const float &b) { return b * a; }

inline Normal operator*(const float &a, const Normal &b) {
  return _mul<Normal, Normal>(a, b);
}

inline Point operator*(const float &a, const Point &b) {
  return _mul<Point, Point>(a, b);
}
inline Point operator*(const Point &a, const float &b) { return b * a; }

/**
 * @brief Minus operation between Vecs and Points
 *
 * @param a
 * @param b
 * @return Vec
 */
inline Vec operator-(const Vec &a, const Vec &b) {
  return _sum<Vec, Vec, Vec>(a, -1 * b);
}
inline Normal operator-(const Normal &a, const Normal &b) {
  return _sum<Normal, Normal, Normal>(a, -1 * b);
}

inline Vec operator-(const Point &a, const Point &b) {
  return _sum<Point, Point, Vec>(a, -1 * b);
}
inline Point operator-(const Point &a, const Vec &b) {
  return _sum<Point, Vec, Point>(a, -1 * b);
}

////////////////////
/*   2D VECTOR    */
////////////////////

/**
 * @brief A 2D vector used to represent a point on a surface
 *
 */
struct Vec2d {
  /**
   * @brief The class members are named u and v to distinguish them from
   * x,y,z, which are used for 3D vectors
   *
   */
  float u, v;
  /**
   * @brief Construct a new Vec 2d object
   *
   * @param _u
   * @param _v
   */
  Vec2d(float _u = 0.f, float _v = 0.f) {
    u = _u;
    v = _v;
  }

  /**
   * @brief Check whether two 2d vectors are the same
   *
   * @param other_vec2d
   * @return true
   * @return false
   */
  inline bool is_close(Vec2d other_vec2d) {
    return are_close(u, other_vec2d.u) && are_close(v, other_vec2d.v);
  }
};

////////////////////////////
/* ORTHOGONAL NORMAL BASE */
////////////////////////////

/**
 * @brief Orthogonal Normal Base (ONB)
 *
 */
struct ONB {
  Vec e1, e2, e3;

  /**
   * @brief Construct a new ONB object: it generate a ONB from a `Normal`
   *
   * @param normal
   */
  ONB(Normal normal);

  /**
   * @brief Construct a new ONB object: it generate a ONB from a generic `Vec`
   *
   * @param vec
   */
  ONB(Vec vec) : ONB(vec.to_norm()) {}
};

////////////////////
/* TRANSFORMATION */
////////////////////

extern float IDENTITY_MATR4x4[4][4];

/** Transformation class
 * @brief Define a generic transformation from two 4x4 matrices
 *
 * @param m A 4x4 matrix
 * @param invm `m` inverse matrix
 */
struct Transformation {
  float m[4][4];
  float invm[4][4];

  /**
   * @brief Construct a new Transformation object
   *
   * @param _m
   * @param _invm
   */
  Transformation(float _m[4][4] = IDENTITY_MATR4x4,
                 float _invm[4][4] = IDENTITY_MATR4x4);

  /**
   * @brief Check if `m` * `invm` is the identity matrix
   */
  bool is_consistent();
  /**
   * @brief Check if two Transformations are the same
   *
   * @return true
   * @return false
   */
  bool is_close(Transformation);

  /**
   * @brief Convert a Transformation to a string
   *
   * @return string
   */
  string to_str();

  /**
   * @brief Return the inverse transformation
   *
   * @return Transformation
   */
  Transformation inverse();
  /**
   * @brief Transformation of a Vec object
   *
   * @param vec
   * @return Vec
   */
  inline Vec operator*(Vec vec) {
    return Vec{vec.x * m[0][0] + vec.y * m[0][1] + vec.z * m[0][2],
               vec.x * m[1][0] + vec.y * m[1][1] + vec.z * m[1][2],
               vec.x * m[2][0] + vec.y * m[2][1] + vec.z * m[2][2]};
  }
  /**
   * @brief Transformation of a Point object
   *
   * @param vec
   * @return Vec
   */
  Point operator*(Point);
  /**
   * @brief Transformation of a Normal object
   *
   * @param n
   * @return Normal
   */
  inline Normal operator*(Normal n) {
    return Normal{n.x * invm[0][0] + n.y * invm[1][0] + n.z * invm[2][0],
                  n.x * invm[0][1] + n.y * invm[1][1] + n.z * invm[2][1],
                  n.x * invm[0][2] + n.y * invm[1][2] + n.z * invm[2][2]};
  }
  /**
   * @brief Transformation composition
   *
   * @return Transformation
   */
  Transformation operator*(Transformation);
};

/**
 * @brief Compare two 4x4 matrices
 *
 * @return true
 * @return false
 */
bool _are_matr_close(float[4][4], float[4][4]);
/**
 * @brief Matrix product between 4x4 matrices
 *
 */
void _matr_prod(const float[4][4], const float[4][4], float[4][4]);

/**
 * @brief Defines a translation
 *
 * @return Transformation
 */
Transformation translation(Vec);
/**
 * @brief Defines a scaling transformation
 *
 * @return Transformation
 */
Transformation scaling(Vec);
/**
 * @brief Define the rotation around the x axis
 *
 * @return Transformation
 */
Transformation rotation_x(float);
/**
 * @brief Define the rotation around the y axis
 *
 * @return Transformation
 */
Transformation rotation_y(float);
/**
 * @brief Define the rotation around the z axis
 *
 * @return Transformation
 */
Transformation rotation_z(float);
#endif