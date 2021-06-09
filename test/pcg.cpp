#include "pcg.h"
#include <cassert>

void test_random() {
  PCG pcg;
  assert(pcg.state == 1753877967969059832);
  assert(pcg.inc == 109);

  uint32_t expected[6] = {2707161783, 2068313097, 3122475824,
                          2211639955, 3215226955, 3421331566};

  for (int i{}; i < 6; i++)
    assert(expected[i] == pcg.random());
}

int main() {
  test_random();

  return 0;
}