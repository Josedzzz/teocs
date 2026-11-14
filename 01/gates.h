#ifndef GATES_H
#define GATES_H

#include "nand.h"

/*
 * Project 01 chips (TECS ch. 1).
 *
 * Buses are arrays of bits, index i = bit i (index 0 is the LSB),
 * like in[0..15] in HDL. Chips with several outputs return them
 * through pointers / output arrays.
 */

/* Elementary gates */
bit Not(bit in);
bit And(bit a, bit b);
bit Or(bit a, bit b);
bit Xor(bit a, bit b);
bit Mux(bit a, bit b, bit sel);
void DMux(bit in, bit sel, bit *a, bit *b);

/* 16-bit variants */
void Not16(const bit in[16], bit out[16]);
void And16(const bit a[16], const bit b[16], bit out[16]);
void Or16(const bit a[16], const bit b[16], bit out[16]);
void Mux16(const bit a[16], const bit b[16], bit sel, bit out[16]);

/* Multi-way variants */
bit Or8Way(const bit in[8]);
void Mux4Way16(const bit a[16], const bit b[16], const bit c[16],
               const bit d[16], const bit sel[2], bit out[16]);
void Mux8Way16(const bit a[16], const bit b[16], const bit c[16],
               const bit d[16], const bit e[16], const bit f[16],
               const bit g[16], const bit h[16], const bit sel[3], bit out[16]);
void DMux4Way(bit in, const bit sel[2], bit *a, bit *b, bit *c, bit *d);
void DMux8Way(bit in, const bit sel[3], bit *a, bit *b, bit *c, bit *d, bit *e,
              bit *f, bit *g, bit *h);

#endif
