#include "./colors.h"
#include <algorithm>
#include <cstdint>
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

struct HdrImage {
  // Variables needed
  int width;
  int height;
  vector<Color> pixels;

  // Default constructor and destructor
  HdrImage(int, int);
  ~HdrImage();

  // Getter and setter methods
  Color get_pixel(int, int);
  void set_pixel(int, int, Color);

  // Misc methods to check integrity
  bool valid_coordinates(int, int);
  int pixel_offset(int, int);
  float endianness(Endianness);

  // Write a float number as its 4 bytes
  void write_float(ostream &, float, Endianness);

  // Methods to write to file on disk or to file on memory
  void write_pfm(ostream &, Endianness);

  // Read a float number as its 4 bytes
  float read_float(istream &, Endianness);
};

class InvalidPfmFileFormat
    : public runtime_error { // classe figlia di std::runtime_error
  using runtime_error::runtime_error;
};

// Reading pfm files methods
Endianness parse_endianness(string);
int *parse_img_size(string);
