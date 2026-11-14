#include "nand.h"

/* The primitive is the only place allowed to use C logic operators. */
bit nand(bit a, bit b) { return (bit) !(a && b); }
