#include "./colors.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// What does this mean?? #TODO: Understand this
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

  // Write a float number as its 4 bytes #TODO: understand this!
  void write_float(ofstream &, float, Endianness);

  // Methods to write to file on disk or to file on memory (overload needed for
  // tests)
  void write_pfm(ofstream &, Endianness);
  void write_pfm(ostringstream &, Endianness);
};
