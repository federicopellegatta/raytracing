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

// read float number as its 4 bytes
float read_float(istream &stream, Endianness e) {
  uint8_t bytes[4];

  for (int i{}; i < 4; i++)
    stream >> noskipws >> bytes[i];

  float value = 0.f;
  if (e == Endianness::little_endian) {
    for (int i{}; i < 4; ++i) {
      // come converto ogni elemento di byte a valore?
    }
  }
  if (e == Endianness::big_endian) {
    for (int i{3}; i >= 0; --i) {
      // come converto ogni elemento di byte a valore?
    }
  }
  return value;
}

// Read pfm file
HdrImage HdrImage::read_pfm(istream &stream) {
  if (!stream)
    throw InvalidPfmFileFormat("File does not exist");

  // seekg ref: https://www.cplusplus.com/reference/istream/istream/seekg/
  // get length of file:
  stream.seekg(0, stream.end);
  int file_length = stream.tellg();
  stream.seekg(0, stream.beg);

  // check file format: is it a PFM file?
  string magic;
  getline(stream, magic);
  if (magic != "PF")
    throw InvalidPfmFileFormat("Invalid magic in PFM file");

  // read img_size
  string img_size;
  getline(stream, img_size);
  int width = parse_img_size(img_size)[0];
  int height = parse_img_size(img_size)[1];

  // read endianness
  string e;
  Endianness endianness;
  getline(stream, e);
  endianness = parse_endianness(e);

  // check if img pixels are >= width*height
  int header_len = stream.tellg();
  if ((file_length - header_len) < (width * height * 3 * 4))
    throw InvalidPfmFileFormat("Invalid file dimension");

  HdrImage results = HdrImage(width, height);
  /*
  // Read the image
  for (int y{height - 1}; y >= 0; y--) {
    for (int x{}; x < width; x++) {
      float r = read_float(stream, endianness);
      float g = read_float(stream, endianness);
      float b = read_float(stream, endianness);
      results.set_pixel(x, y, Color{r, g, b});
    }
  }
  */
  return results;
}

// reading file pfm methodsstof()
Endianness parse_endianness(string str) {

  float floatEndianness = 0.f;
  try {
    floatEndianness = stof(str);
  } catch (invalid_argument) {
    throw InvalidPfmFileFormat("Missing endianness specification");
  }

  if (floatEndianness == -1.0) { // perchè se scrivo floatEndianness == -1.0
                                 // dice che non ho definito floatEndianness?
                                 // Simone:
                                 // Perchè l'avevi solamente definito
                                 // all'interno dello scope del try
    return Endianness::little_endian;
  } else if (floatEndianness == 1.0) {
    return Endianness::big_endian;
  } else {
    throw InvalidPfmFileFormat("Invalid endianness specification");
  }
}

vector<int> parse_img_size(string str) {
  string w = str.substr(0, str.find(" "));
  string h = str.erase(0, w.size() + 1).substr(0, str.find(" "));
  string rest = str.erase(0, w.size() + 1);

  int width = 0;
  int height = 0;
  try {
    if (rest == "") {
      width = stoi(w); // nota: stoi(1.1)=1
      height = stoi(h);
    } else {
      throw InvalidPfmFileFormat(
          "Invalid image dimension: more than 2 dimensions");
    }

    if (width < 0 || height < 0) {
      throw InvalidPfmFileFormat(
          "Invalid image dimension: width/height is negative integer");
    }
  } catch (invalid_argument) {
    throw InvalidPfmFileFormat(
        "Invalid image dimension: width/height not a integer");
  }

  vector<int> dim = {width, height};
  return dim;
}