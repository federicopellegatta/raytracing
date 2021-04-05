#include "../include/HdrImage.h"
#include <cassert>

using namespace std;

void test_image_creation(HdrImage img, int x, int y) {
  assert(img.width == x);
  assert(img.height == y);
}

void test_coordinates(HdrImage img) {
  assert(img.valid_coordinates(0, 0));
  assert(img.valid_coordinates(6, 3));
  assert(img.valid_coordinates(-1, 0) == false);
  assert(img.valid_coordinates(0, -1) == false);
}

void test_pixel_offset(HdrImage img) {
  assert(img.pixel_offset(0, 0) == 0);
  assert(img.pixel_offset(3, 2) == 17);
  assert(img.pixel_offset(6, 3) == 7 * 4 - 1);
}

void test_set_get_pixel(HdrImage img, Color color, int x, int y) {
  assert(color.is_close(img.get_pixel(x, y)));
}

void test_pfm_save(HdrImage img) { // SOLO little_endian
  // set pixels of my image
  img.set_pixel(0, 0, Color(1.0e1, 2.0e1, 3.0e1));
  img.set_pixel(1, 0, Color(4.0e1, 5.0e1, 6.0e1));
  img.set_pixel(2, 0, Color(7.0e1, 8.0e1, 9.0e1));
  img.set_pixel(0, 1, Color(1.0e2, 2.0e2, 3.0e2));
  img.set_pixel(1, 1, Color(4.0e2, 5.0e2, 6.0e2));
  img.set_pixel(2, 1, Color(7.0e2, 8.0e2, 9.0e2));

  // little_endian reference pfm and compare it with my image
  vector<unsigned char> reference_le_pfm = {
      0x50, 0x46, 0x0a, 0x33, 0x20, 0x32, 0x0a, 0x2d, 0x31, 0x2e, 0x30, 0x0a,
      0x00, 0x00, 0xc8, 0x42, 0x00, 0x00, 0x48, 0x43, 0x00, 0x00, 0x96, 0x43,
      0x00, 0x00, 0xc8, 0x43, 0x00, 0x00, 0xfa, 0x43, 0x00, 0x00, 0x16, 0x44,
      0x00, 0x00, 0x2f, 0x44, 0x00, 0x00, 0x48, 0x44, 0x00, 0x00, 0x61, 0x44,
      0x00, 0x00, 0x20, 0x41, 0x00, 0x00, 0xa0, 0x41, 0x00, 0x00, 0xf0, 0x41,
      0x00, 0x00, 0x20, 0x42, 0x00, 0x00, 0x48, 0x42, 0x00, 0x00, 0x70, 0x42,
      0x00, 0x00, 0x8c, 0x42, 0x00, 0x00, 0xa0, 0x42, 0x00, 0x00, 0xb4, 0x42};

  // write test on memory
  ostringstream buffer;
  img.write_pfm(buffer, Endianness::little_endian);

  assert(buffer.str().size() == reference_le_pfm.size()); // controllo dim

  for (int i{}; i < reference_le_pfm.size(); i++) {
    uint8_t byte = (uint8_t)buffer.str()[i];
    assert(byte == reference_le_pfm[i]);
  }

  ofstream outputFile("./my_le_img.pfm");
  img.write_pfm(outputFile, Endianness::little_endian);
}

void test_pfm_parse_endianness() {
  assert(parse_endianness("-1.0") == Endianness::little_endian);
  assert(parse_endianness("1.0") == Endianness::big_endian);

  try {
    parse_endianness("2.0");
  } catch (InvalidPfmFileFormat err) {
    // printf("Caught exception: %s \n", err.what());
  }

  try {
    parse_endianness("abc");
  } catch (InvalidPfmFileFormat err) {
    // printf("Caught exception: %s \n", err.what());
  }
}

void test_pfm_parse_img_size() {
  int dim[2] = {3, 2};

  for (int i = 0; i < 2; i++) {
    assert(dim[i] == parse_img_size("3 2")[i]);
  }

  try {
    parse_img_size("-1 3");
  } catch (InvalidPfmFileFormat err) {
    // printf("Caught exception: %s \n", err.what());
  }

  try {
    parse_img_size("1 4 c");
  } catch (InvalidPfmFileFormat err) {
    // printf("Caught exception: %s \n", err.what());
  };
  try {
    parse_img_size("1 c");
  } catch (InvalidPfmFileFormat err) {
    // printf("Caught exception: %s \n", err.what());
  }
}

void test_pfm_read() {
  ifstream le_in("../test/HdrImage_references/reference_le.pfm");
  ifstream be_in("../test/HdrImage_references/reference_be.pfm");

  HdrImage img_le(le_in);
  HdrImage img_be(be_in);
  // HdrImage img_le("../test/HdrImage_references/reference_le.pfm");
  // HdrImage img_be("../test/HdrImage_references/reference_be.pfm");
  // Little endian image tests
  assert(img_le.width == 3);
  assert(img_le.height == 2);

  assert(img_le.get_pixel(0, 0).is_close(Color(1.0e1, 2.0e1, 3.0e1)));
  assert(img_le.get_pixel(1, 0).is_close(Color(4.0e1, 5.0e1, 6.0e1)));
  assert(img_le.get_pixel(2, 0).is_close(Color(7.0e1, 8.0e1, 9.0e1)));
  assert(img_le.get_pixel(0, 1).is_close(Color(1.0e2, 2.0e2, 3.0e2)));
  assert(img_le.get_pixel(0, 0).is_close(Color(1.0e1, 2.0e1, 3.0e1)));
  assert(img_le.get_pixel(1, 1).is_close(Color(4.0e2, 5.0e2, 6.0e2)));
  assert(img_le.get_pixel(2, 1).is_close(Color(7.0e2, 8.0e2, 9.0e2)));

  // Big endian image tests
  assert(img_be.width == 3);
  assert(img_be.height == 2);

  assert(img_be.get_pixel(0, 0).is_close(Color(1.0e1, 2.0e1, 3.0e1)));
  assert(img_be.get_pixel(1, 0).is_close(Color(4.0e1, 5.0e1, 6.0e1)));
  assert(img_be.get_pixel(2, 0).is_close(Color(7.0e1, 8.0e1, 9.0e1)));
  assert(img_be.get_pixel(0, 1).is_close(Color(1.0e2, 2.0e2, 3.0e2)));
  assert(img_be.get_pixel(0, 0).is_close(Color(1.0e1, 2.0e1, 3.0e1)));
  assert(img_be.get_pixel(1, 1).is_close(Color(4.0e2, 5.0e2, 6.0e2)));
  assert(img_be.get_pixel(2, 1).is_close(Color(7.0e2, 8.0e2, 9.0e2)));
}

void test_average_luminosity() {
  HdrImage img(2, 1);
  img.set_pixel(0, 0, Color(5.0, 10.0, 15.0));       // Luminosity : 10.0
  img.set_pixel(1, 0, Color(500.0, 1000.0, 1500.0)); // Luminosity : 1000.0

  assert(are_close(100.0, img.average_luminosity(0.0)));
}

void test_clamp_image() {
  HdrImage img(2, 1);
  img.set_pixel(0, 0, Color(5.0, 10.0, 15.0));       // Luminosity : 10.0
  img.set_pixel(1, 0, Color(500.0, 1000.0, 1500.0)); // Luminosity : 1000.0

  img.clamp_image();

  // Just check that the R / G / B values are within the expected boundaries
  for (int i{}; i < img.pixels.size(); i++) {
    assert((img.pixels[i].r >= 0) && (img.pixels[i].r <= 1));
    assert((img.pixels[i].r >= 0) && (img.pixels[i].r <= 1));
    assert((img.pixels[i].r >= 0) && (img.pixels[i].r <= 1));
  }
}

void test_normalize_image(bool test_overload) {
  HdrImage img(2, 1);

  img.set_pixel(0, 0, Color(5.0, 10.0, 15.0));
  img.set_pixel(1, 0, Color(500.0, 1000.0, 1500.0));

  // Test NormalizeImage
  if (test_overload == false) {
    img.normalize_image(1000.0);
    assert(img.get_pixel(0, 0).is_close(Color(0.5e2, 1.0e2, 1.5e2)));
    assert(img.get_pixel(1, 0).is_close(Color(0.5e4, 1.0e4, 1.5e4)));
  } else {
    // Testing overload of NormalizeImage
    img.normalize_image(1000.0, 100.0);
    assert(img.get_pixel(0, 0).is_close(Color(0.5e2, 1.0e2, 1.5e2)));
    assert(img.get_pixel(1, 0).is_close(Color(0.5e4, 1.0e4, 1.5e4)));
  }
}

int main() {

  HdrImage img(7, 4);
  HdrImage img1(3, 2);
  Color reference_color(1.0, 2.0, 3.0);

  // Testing if set and get pixel works properly
  img.set_pixel(3, 2, reference_color);

  test_image_creation(img, 7, 4);
  test_coordinates(img);
  test_pixel_offset(img);
  test_set_get_pixel(img, reference_color, 3, 2);

  test_pfm_save(img1);

  test_pfm_parse_endianness();
  test_pfm_parse_img_size();
  test_pfm_read();

  test_average_luminosity();
  test_normalize_image(false);
  test_normalize_image(true);
  test_clamp_image();

  return 0;
}