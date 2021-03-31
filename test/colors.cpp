#include "../include/colors.h"
#include <cassert>
#include <cstdlib>

int main() {
  Color c1(1.0, 2.0, 3.0);
  Color c2(5.0, 7.0, 9.0);
  float a = 3.0;

  Color c3(6.0, 9.0, 12.0);
  Color c4(3.0, 6.0, 9.0);
  Color c5(5.0, 14.0, 27.0);

  assert(c3.is_close(c1 + c2)); // check colors sum
  assert(c4.is_close(c1 * a));  // check product color by float
  assert(c5.is_close(c1 * c2)); // check product color by color

  assert(approx(2.0, c1.luminosity()));
  assert(approx(7.0, c2.luminosity()));

  return 0;
}