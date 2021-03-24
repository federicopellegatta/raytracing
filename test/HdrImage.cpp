#include "../include/HdrImage.h"
#include <cassert>

using namespace std;

void test_image_creation(HdrImage img, unsigned int x, unsigned int y) {
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
  /*
  unsigned char _reference_le_pfm[] = {
      0x50, 0x46, 0x0a, 0x33, 0x20, 0x32, 0x0a, 0x2d, 0x31, 0x2e, 0x30, 0x0a,
      0x00, 0x00, 0xc8, 0x42, 0x00, 0x00, 0x48, 0x43, 0x00, 0x00, 0x96, 0x43,
      0x00, 0x00, 0xc8, 0x43, 0x00, 0x00, 0xfa, 0x43, 0x00, 0x00, 0x16, 0x44,
      0x00, 0x00, 0x2f, 0x44, 0x00, 0x00, 0x48, 0x44, 0x00, 0x00, 0x61, 0x44,
      0x00, 0x00, 0x20, 0x41, 0x00, 0x00, 0xa0, 0x41, 0x00, 0x00, 0xf0, 0x41,
      0x00, 0x00, 0x20, 0x42, 0x00, 0x00, 0x48, 0x42, 0x00, 0x00, 0x70, 0x42,
      0x00, 0x00, 0x8c, 0x42, 0x00, 0x00, 0xa0, 0x42, 0x00, 0x00, 0xb4, 0x42};
  unsigned int reference_le_pfm_len = 84;
  */
  vector<unsigned char> reference_le_pfm = {
      0x50, 0x46, 0x0a, 0x33, 0x20, 0x32, 0x0a, 0x2d, 0x31, 0x2e, 0x30, 0x0a,
      0x00, 0x00, 0xc8, 0x42, 0x00, 0x00, 0x48, 0x43, 0x00, 0x00, 0x96, 0x43,
      0x00, 0x00, 0xc8, 0x43, 0x00, 0x00, 0xfa, 0x43, 0x00, 0x00, 0x16, 0x44,
      0x00, 0x00, 0x2f, 0x44, 0x00, 0x00, 0x48, 0x44, 0x00, 0x00, 0x61, 0x44,
      0x00, 0x00, 0x20, 0x41, 0x00, 0x00, 0xa0, 0x41, 0x00, 0x00, 0xf0, 0x41,
      0x00, 0x00, 0x20, 0x42, 0x00, 0x00, 0x48, 0x42, 0x00, 0x00, 0x70, 0x42,
      0x00, 0x00, 0x8c, 0x42, 0x00, 0x00, 0xa0, 0x42, 0x00, 0x00, 0xb4, 0x42};
  // reference_le_pfm.push_back(_reference_le_pfm);

  // cout << reference_le_pfm.size() << endl;
  // write test on memory
  ostringstream buffer;
  img.write_pfm(buffer, Endianness::little_endian);
  // cout << buffer.str() << endl;
  assert(buffer.str().size() == reference_le_pfm.size());

  pair<string::iterator, vector<unsigned char>::iterator> expected_pair(
      buffer.str().end(), reference_le_pfm.end());
  pair<string::iterator, vector<unsigned char>::iterator> result_pair;
  result_pair = mismatch(buffer.str().begin(), buffer.str().end(),
                         reference_le_pfm.begin());

  cout << *buffer.str().end() << "\n" << *reference_le_pfm.end() << endl;
  // cout << *(result_pair.first) << "\n" << *(result_pair.second) << endl;
  // assert(expected_pair == result_pair);
  ofstream outputFile("./my_le_img.pfm");

  img.write_pfm(outputFile, Endianness::little_endian);
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

  return 0;
}