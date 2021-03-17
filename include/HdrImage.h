#include "./colors.h"
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct HdrImage {
  unsigned int width, height;
  vector<Color> pixels;

  HdrImage(unsigned int, unsigned int);
  ~HdrImage();

  Color get_pixel(unsigned int, unsigned int);
  void set_pixel(unsigned int, unsigned int, Color);
  void write_pfm(ofstream &stream, float value);
  void write_pfm(ostringstream &stream, float value);

  bool valid_coordinates(unsigned int, unsigned int);
  unsigned int pixel_offset(unsigned int, unsigned int);
};
