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

bit Mux(bit a, bit b, bit sel) {
  bit x1 = And(a, Not(sel));
  bit x2 = And(b, sel);
  return Or(x1, x2);
}

void DMux(bit in, bit sel, bit *a, bit *b) {
  *a = And(in, Not(sel));
  *b = And(in, sel);
}

void Not16(const bit *in, bit *out) {
  for (int i = 0; i < 16; i++) {
    out[i] = Not(in[i]);
  }
}
