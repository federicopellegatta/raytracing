#include "pcg.h"

PCG::PCG(uint64_t init_state, uint64_t init_seq) {
  inc = (init_seq << 1) | 1;
  random(); // Throw a random number and discard it
  state += init_state;
  random(); // Throw a random number and discard it
}

uint32_t PCG::random() {
  uint64_t old_state = state;
  state = old_state * 6364136223846793005 + inc;

  uint32_t xorshifted = ((old_state >> 18) ^ old_state) >> 27;
  uint32_t rot = old_state >> 59;

  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}