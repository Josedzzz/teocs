#ifndef ARITH_H
#define ARITH_H

#include "../01/gates.h" /* project 01 chips (and nand.h) */

/*
 * Project 02 chips (TECS ch. 2).
 *
 * Same conventions as project 01: buses are arrays of bits, index 0 is the
 * LSB. Numbers are 16-bit two's complement. Chips with several outputs
 * return them through pointers / output arrays.
 */

/* Adders */
void HalfAdder(bit a, bit b, bit *sum, bit *carry);
void FullAdder(bit a, bit b, bit c, bit *sum, bit *carry);
void Add16(const bit a[16], const bit b[16], bit out[16]);
void Inc16(const bit in[16], bit out[16]);

/*
 * ALU: computes out from x and y according to the six control bits
 * zx, nx, zy, ny, f, no. Also sets zr = 1 if out == 0, ng = 1 if out < 0.
 */
void ALU(const bit x[16], const bit y[16], bit zx, bit nx, bit zy, bit ny,
         bit f, bit no, bit out[16], bit *zr, bit *ng);

#endif
