#include "./colors.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

enum class Endianness { little_endian, big_endian };

struct HdrImage {
  int width;
  int height;
  vector<Color> pixels;

  HdrImage(int, int);
  ~HdrImage();

  Color get_pixel(int, int);
  void set_pixel(int, int, Color);
  void write_pfm(ofstream &stream, Endianness endianness);
  // void write_pfm(ostringstream &stream, Endianness endianness);
  void
  write_float(ofstream &stream, float value,
              Endianness endianness); // scompone un float nei suoi quattro byte

  bool valid_coordinates(int, int);
  int pixel_offset(int, int);
  float endianness(Endianness);
};
