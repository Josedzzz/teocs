#include "gates.h"
#include "nand.h"

/* Implement the chips declared in gates.h here, using only nand()
 * and chips you have already built. */

bit Not(bit in) { return nand(in, in); }

bit And(bit a, bit b) { return Not(nand(a, b)); }

bit Or(bit a, bit b) { return nand(Not(a), Not(b)); }

bit Xor(bit a, bit b) {
  bit x1 = nand(a, Not(b));
  bit x2 = nand(Not(a), b);
  return nand(x1, x2);
}
