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

#include "HdrImage.h"

void HdrImage::read_pfm(istream &stream) {
  if (!stream)
    throw ios_base::failure("File does not exist");

  // get length of file:
  stream.seekg(0, stream.end);
  int file_len = stream.tellg();
  stream.seekg(0, stream.beg);
  if (file_len == -1)
    throw InvalidPfmFileFormat("The file is empty");

  // check file format: is it a PFM file?
  string magic;
  getline(stream, magic);
  if (magic != "PF")
    throw InvalidPfmFileFormat("Invalid magic in PFM file");

  // read img_size
  string img_size;
  getline(stream, img_size);
  width = parse_img_size(img_size)[0];
  height = parse_img_size(img_size)[1];

  // read endianness
  string e;
  Endianness endianness;
  getline(stream, e);
  endianness = parse_endianness(e);

  // check if img pixels are >= width*height
  int header_len = stream.tellg();
  if ((file_len - header_len) < (width * height * 3 * 4))
    throw InvalidPfmFileFormat("Invalid file dimension");

  allocate_memory(width, height);
  // Read the image
  for (int y{height - 1}; y >= 0; y--) {
    for (int x{}; x < width; x++) {
      float r = read_float(stream, endianness);
      float g = read_float(stream, endianness);
      float b = read_float(stream, endianness);
      set_pixel(x, y, Color(r, g, b));
    }
  }
}

HdrImage::HdrImage(int w, int h) { allocate_memory(w, h); }

HdrImage::HdrImage(const string &file_name) {
  ifstream stream{file_name};
  read_pfm(stream);
}

HdrImage::HdrImage(istream &stream) { read_pfm(stream); };

bool HdrImage::valid_coordinates(int x, int y) {
  if (x >= 0 && x < width && y >= 0 && y < height) {
    return true;
  } else {
    return false;
  }
}

void HdrImage::set_pixel(int x, int y, Color color) {
  if (valid_coordinates(x, y)) {
    pixels[pixel_offset(x, y)] = color;
  } else {
    fmt::print("Couldn't set pixel because coordinates are not valid!\n");
  }
}

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

void HdrImage::write_pfm(ostream &stream, Endianness e) {
  stringstream sstr;

  float float_endianness;
  if (e == Endianness::little_endian) {
    float_endianness = -1.f;
  } else {
    float_endianness = 1.f;
  }

  // fixed and setprecision in order to have .0 in output
  sstr << "PF\n"
       << width << " " << height << "\n"
       << fixed << setprecision(1) << float_endianness;

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

float HdrImage::read_float(istream &stream, Endianness e) {
  unsigned char bytes[4];

  for (int i{}; i < 4; i++)
    stream >> noskipws >> bytes[i];

  float value = 0.f;
  if (e == Endianness::little_endian)
    memcpy(&value, &bytes, sizeof(value));

  if (e == Endianness::big_endian) {
    reverse(begin(bytes), end(bytes));
    memcpy(&value, &bytes, sizeof(value));
  }
  return value;
}

// reading file pfm method
Endianness parse_endianness(string str) {

  float floatEndianness = 0.f;
  try {
    floatEndianness = stof(str);
  } catch (invalid_argument) {
    throw InvalidPfmFileFormat("Missing endianness specification");
  }

  if (floatEndianness == -1.0) {
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
          "Invalid image dimension: width and/or height is negative integer");
    }
  } catch (invalid_argument) {
    throw InvalidPfmFileFormat(
        "Invalid image dimension: width and/or height not a integer");
  }

  vector<int> dim = {width, height};
  return dim;
}

float HdrImage::average_luminosity(float delta) {
  float cumsum = 0.f;
  for (int i{}; i < pixels.size(); i++)
    cumsum += log10(delta + pixels[i].luminosity());

  return pow(10, cumsum / pixels.size());
};

void HdrImage::clamp_image() {
  for (int i{}; i < pixels.size(); i++) {
    pixels[i].r = clamp(pixels[i].r);
    pixels[i].g = clamp(pixels[i].g);
    pixels[i].b = clamp(pixels[i].b);
  }
}

void HdrImage::normalize_pixels(float factor, float luminosity) {
  if (factor <= 0)
    throw invalid_argument("FACTOR must be a positive floating point");

  for (int i{}; i < pixels.size(); i++) {
    pixels[i] = pixels[i] * (factor / luminosity);
  }
}

// Write LDR image
void HdrImage::write_ldr_image(const char *output_filename, float gamma) {
  gdImagePtr img;
  FILE *output_file;

  img = gdImageCreateTrueColor(width, height);
  if (gamma <= 0)
    throw invalid_argument("GAMMA must be a positive floating point");

  for (int row{}; row < height; ++row) {
    for (int col{}; col < width; ++col) {
      Color cur_color = get_pixel(col, row);
      int red, green, blue;
      red = static_cast<int>(255 * pow(cur_color.r, 1 / gamma));
      green = static_cast<int>(255 * pow(cur_color.g, 1 / gamma));
      blue = static_cast<int>(255 * pow(cur_color.b, 1 / gamma));
      gdImageSetPixel(img, col, row, gdImageColorExact(img, red, green, blue));
    }
  }

  output_file = fopen(output_filename, "wb");
  if (!output_file) {
    throw ios_base::failure("Failed to open output file.");
  }

  string format =
      static_cast<string>(output_filename)
          .erase(0, static_cast<string>(output_filename).find(".") + 1);

  if (format == "png") {
    gdImagePng(img, output_file);
  } else if (format == "jpeg" || format == "jpg") {
    gdImageJpeg(img, output_file, -1);
  } else {
    throw runtime_error("Format is not supported");
  }

  fclose(output_file);
  gdImageDestroy(img);
}