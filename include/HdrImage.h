#include "./colors.h"
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

struct HdrImage {
private:
  // Read a pfm file
  void read_pfm(istream &);
  void allocate_memory(int, int);

public:
  // Variables needed
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
  float average_luminosity() { return average_luminosity(1e-10); };
};

// classe figlia di std::runtime_error
class InvalidPfmFileFormat : public runtime_error {
  using runtime_error::runtime_error;
};

// Reading pfm files methods
Endianness parse_endianness(string);
vector<int> parse_img_size(string);
