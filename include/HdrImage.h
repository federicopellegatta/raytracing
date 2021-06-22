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

#ifndef HDRIMAGE_H
#define HDRIMAGE_H

#include "colors.h"
#include "gd.h"
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/**
 * @brief Class representing the two possible endian values.
 *
 */
enum class Endianness { little_endian, big_endian };

/**
 * @brief Parse endianess of a pfm file and return either big or little endian
 *
 * @return Endianness
 */
Endianness parse_endianness(string);
/**
 * @brief Parse ofm file to get image size
 *
 * @return vector<int>
 */
vector<int> parse_img_size(string);

/**
 * @brief Clamp the colors of the image
 *
 * @param x
 * @return float
 */
inline float clamp(float x) { return x / (1 + x); }

/**
 * @brief Derived class for error management
 *
 */
class InvalidPfmFileFormat : public runtime_error {
  using runtime_error::runtime_error;
};

/** HdrImage class
 * @brief This class represents a HDR (High-Dynamic Range) image
 *
 * @param width The number of pixel on the horizontal axis
 * @param height The number of pixel on the vertical axis
 * @param pixels a :class:`.Color` vector that contains all pixels' color
 */
struct HdrImage {
private:
  /**
   * @brief Read a pfm file
   *
   */
  void read_pfm(istream &);
  /**
   * @brief Allocate an image in memory, initializing width, height and
    pixels.size
   *
   * @param w
   * @param h
   */
  inline void allocate_memory(int w, int h) {
    width = w;
    height = h;
    pixels.resize(w * h);
  }
  /**
   * @brief Normalize the pixels
   *
   */
  void normalize_pixels(float, float);

public:
  int width;
  int height;
  vector<Color> pixels;

  /**
   * @brief Construct a new Hdr Image object from two integers (width and
   * height)
   *
   */
  HdrImage(int, int);
  /**
   * @brief Construct a new Hdr Image object using an input file
   *
   */
  HdrImage(const string &);
  /**
   * @brief Construct a new Hdr Image object from a input stream
   *
   */
  HdrImage(istream &);
  /**
   * @brief Destroy the Hdr Image object
   *
   */
  ~HdrImage(){};

  /**
   * @brief Converts matrix like position in 1D vector position (needed by
   * vector<Colors> pixels)
   * @param x
   * @param y
   * @return int
   */
  inline int pixel_offset(int x, int y) { return y * width + x; }
  /**
   * @brief Get the pixel object
   *
   * @param x
   * @param y
   * @return Color
   */
  inline Color get_pixel(int x, int y) { return pixels[pixel_offset(x, y)]; }

  /**
   * @brief Assign color to target pixel
   *
   */
  void set_pixel(int, int, Color);

  /**
   * @brief Check if x and y are positive integers, and are inside the matrix
   *
   * @return true
   * @return false
   */
  bool valid_coordinates(int, int);

  /**
   * @brief Write a float number as its 4 bytes
   *
   */
  void write_float(ostream &, float, Endianness);

  /**
   * @brief Methods to write to file on disk or to file on memory
   *
   */
  void write_pfm(ostream &, Endianness);

  /**
   * @brief Read a float number as its 4 bytes
   *
   * @return float
   */
  float read_float(istream &, Endianness);

  /**
   * @brief Calculates the average luminosity of the image
   *
   * @return float
   */
  float average_luminosity(float);
  /**
   * @brief Overload, calls `average_luminosity` with fixed delta = 1e-10
   *
   * @return float
   */
  inline float average_luminosity() { return average_luminosity(1e-10); }

  /**
   * @brief Normalize the image by `factor`. Calculates internally the
   * luminosity to use.
   *
   * @param factor
   */
  inline void normalize_image(float factor) {
    float luminosity = average_luminosity();
    normalize_pixels(factor, luminosity);
  }
  /**
   * @brief Overload: accepts a luminosity parameter
   *
   * @param factor
   * @param luminosity
   */
  inline void normalize_image(float factor, float luminosity) {
    normalize_pixels(factor, luminosity);
  }

  /**
   * @brief clamp luminosity spot
   *
   */
  void clamp_image();

  /**
   * @brief Convert PFM to a LDR format
   *
   */
  void write_ldr_image(const char *, float);
};
#endif