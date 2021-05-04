#include "colors.h"
#include "gd.h"
#include <algorithm>
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

enum class Endianness { little_endian, big_endian };

/** HdrImage class
 * @brief This class represents a HDR (High-Dynamic Range) image
 *
 * @param width The number of pixel on the horizontal axis
 * @param height The number of pixel on the vertical axis
 * @param pixels a :class:`.Color` vector that contains all pixels' color
 */
struct HdrImage {
private:
  // Read a pfm file
  void read_pfm(istream &);
  // Allocate an image in memory,
  // initializing width, height and pixels.size()
  void allocate_memory(int, int);
  // Normalize the pixels
  // Needed for NormalizeImage and its overload
  void normalize_pixels(float, float);

public:
  int width;
  int height;
  vector<Color> pixels;

  HdrImage(int, int);
  HdrImage(const string &);
  HdrImage(istream &);
  ~HdrImage();

  // Getter and setter methods
  Color get_pixel(int, int);
  void set_pixel(int, int, Color);

  // Misc methods to check integrity
  bool valid_coordinates(int, int);
  int pixel_offset(int, int);

  // Write a float number as its 4 bytes
  void write_float(ostream &, float, Endianness);

  // Methods to write to file on disk or to file on memory
  void write_pfm(ostream &, Endianness);

  // Read a float number as its 4 bytes
  float read_float(istream &, Endianness);

  float average_luminosity(float);
  float average_luminosity();

  // Normalize Image (needed for conversion to LDR)
  // This accept only the factor, and calculates the luminosity
  void normalize_image(float);
  // Overload that accepts a luminosity parameter
  void normalize_image(float, float);

  // clamp luminosity spot
  void clamp_image();

  // Convert PFM to a LDR format
  void write_ldr_image(const char *, float);
};

// classe figlia di std::runtime_error
class InvalidPfmFileFormat : public runtime_error {
  using runtime_error::runtime_error;
};

// Reading pfm files methods
Endianness parse_endianness(string);
vector<int> parse_img_size(string);

float clamp(float);
