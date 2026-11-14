#ifndef NAND_H
#define NAND_H

/* A single wire: always 0 or 1. */
typedef unsigned char bit;

/* The only primitive. Everything else is built from this. */
bit nand(bit a, bit b);

#endif
