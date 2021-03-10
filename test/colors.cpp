#include "../include/colors.h"
#include <cstdlib>

int main() {
  Color c1(1.0, 2.0, 3.0);
  Color c2(5.0, 7.0, 9.0);
  Color c3(6.0, 9.0, 11.0);

  return c3.is_close(c1 + c2) ? EXIT_SUCCESS : EXIT_FAILURE;
}