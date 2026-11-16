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

void And16(const bit *a, const bit *b, bit *out) {
  for (int i = 0; i < 16; i++) {
    out[i] = And(a[i], b[i]);
  }
}

void Or16(const bit *a, const bit *b, bit *out) {
  for (int i = 0; i < 16; i++) {
    out[i] = Or(a[i], b[i]);
  }
}

void Mux16(const bit *a, const bit *b, bit sel, bit *out) {
  for (int i = 0; i < 16; i++) {
    out[i] = Mux(a[i], b[i], sel);
  }
}

bit Or8Way(const bit *in) {
  bit out = in[0];
  for (int i = 1; i < 8; i++) {
    out = Or(out, in[i]);
  }
  return out;
}

void Mux4Way16(const bit *a, const bit *b, const bit *c, const bit *d,
               const bit *sel, bit *out) {
  bit ab[16], cd[16];
  Mux16(a, b, sel[0], ab);    /* sel[0] picks a or b         */
  Mux16(c, d, sel[0], cd);    /* sel[0] picks c or d         */
  Mux16(ab, cd, sel[1], out); /* sel[1] picks the pair (top/bottom) */
}
