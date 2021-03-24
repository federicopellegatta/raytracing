#include "../include/HdrImage.h"

using namespace std;

// Default constructor
HdrImage::HdrImage(int w, int h) : pixels(w * h) {
  width = w;
  height = h;
}

// Default destructor
HdrImage::~HdrImage(){};

// Valid coordinates implementation
// Check if x and y are positive integers, and are inside the matrix
bool HdrImage::valid_coordinates(int x, int y) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    return true;
  } else {
    return false;
  }
}

// Converts matrix like position in 1D vector position (needed by vector<Colors>
// pixels)
int HdrImage::pixel_offset(int x, int y) { return y * width + x; }

// Assign color to target pixel
void HdrImage::set_pixel(int x, int y, Color color) {
  if (valid_coordinates(x, y)) {
    pixels[pixel_offset(x, y)] = color;
  } else {
    cout << "Couldn't set pixel because coordinates are not valid!" << endl;
  }
}

// Getter implementation
Color HdrImage::get_pixel(int x, int y) { return pixels[pixel_offset(x, y)]; }

void HdrImage::write_float(ostream &stream, float value,
                           Endianness endianness) {
  // Convert "value" in a sequence of 32 bit
  uint32_t double_word{*((uint32_t *)&value)};

  // Extract the four bytes in "double_word" using bit-level operators
  uint8_t bytes[] = {
      static_cast<uint8_t>(double_word & 0xFF), // Least significant byte
      static_cast<uint8_t>((double_word >> 8) & 0xFF),
      static_cast<uint8_t>((double_word >> 16) & 0xFF),
      static_cast<uint8_t>((double_word >> 24) & 0xFF), // Most significant byte
  };

  switch (endianness) {
  case Endianness::little_endian:
    for (int i{}; i < 4; ++i) // Forward loop
      stream << bytes[i];
    break;

  case Endianness::big_endian:
    for (int i{3}; i >= 0; --i) // Backward loop
      stream << bytes[i];
    break;
  }
}

float HdrImage::endianness(Endianness e) {
  if (e == Endianness::little_endian) {
    return -1.0;
  } else {
    return 1.0;
  }
}

void HdrImage::write_pfm(ostream &stream, Endianness e) {
  stringstream sstr;

  // fixed and setprecision in order to have .0 in output
  // see https://www.cplusplus.com/reference/iomanip/setprecision/
  sstr << "PF\n"
       << width << " " << height << "\n"
       << fixed << setprecision(1) << endianness(e);
  string result{sstr.str()};

  stream << result << endl;
  for (int y = height - 1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      Color color = get_pixel(x, y);
      write_float(stream, color.r, e);
      write_float(stream, color.g, e);
      write_float(stream, color.b, e);
    }
  }
}
/*
void HdrImage::write_pfm(ostringstream &sstream, Endianness e) {
  // stringstream sstr;

  sstream << "PF\n" << width << " " << height << "\n" << endianness(e);
  // string result{sstr.str()};

  // sstream << result << endl;
  for (int y = height - 1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      Color color = get_pixel(x, y);
      write_float(sstream, color.r, e);
      write_float(sstream, color.g, e);
      write_float(sstream, color.b, e);
    }
  }
}
*/