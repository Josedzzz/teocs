#include "arith.h"

/* Implement the chips declared in arith.h here, using only nand(), the
 * project 01 chips and chips you have already built. */

void HalfAdder(bit a, bit b, bit *sum, bit *carry) {
  *sum = Xor(a, b);
  *carry = And(a, b);
}
