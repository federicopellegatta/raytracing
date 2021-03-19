#include "./colors.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct HdrImage {
  int width;
  int height;
  vector<Color> pixels;

  HdrImage(int, int);
  ~HdrImage();

  Color get_pixel(int, int);
  void set_pixel(int, int, Color);
  void write_pfm(ofstream &stream, float value);
  void write_pfm(ostringstream &stream, float value);

  bool valid_coordinates(int, int);
  int pixel_offset(int, int);
};
