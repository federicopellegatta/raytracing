#include "../include/colors.h"
#include <cstdlib>

int main() {
  Color c1(1.0, 2.0, 3.0);
  Color c2(5.0, 7.0, 9.0);
  float a = 3.0;

  Color c3(6.0, 9.0, 12.0);
  Color c4(3.0, 6.0, 9.0);
  Color c5(5.0, 14.0, 27.0);

  return (c3.is_close(c1 + c2) && c4.is_close(c1 * a) && c5.is_close(c1 * c2))
             ? EXIT_SUCCESS
             : EXIT_FAILURE;
  // return c3.is_close(c1 + c2) ? EXIT_SUCCESS : EXIT_FAILURE;
}