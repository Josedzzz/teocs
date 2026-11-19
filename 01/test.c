/*
 * Tests for the project 01 chips.
 *
 * Each test runs a chip over its whole truth table and compares the
 * result with a reference value computed with plain C operators.
 * (C operators are fine HERE: this is test code, not chip code.)
 */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "gates.h"

static int checks = 0;
static int failures = 0;

/* Compare one chip output with the expected value; report on mismatch. */
static void expect(const char *what, bit got, bit expected) {
  checks++;
  if (got != expected) {
    failures++;
    printf("FAIL %s: got %d, expected %d\n", what, got, expected);
  }
}

/* Spread a 16-bit number over a bus: bus[i] = bit i of v (bus[0] = LSB). */
static void to_bus(uint16_t v, bit bus[16]) {
  for (int i = 0; i < 16; i++) {
    bus[i] = (v >> i) & 1;
  }
}

/* Interesting 16-bit inputs, written in hexadecimal (1 hex digit = 4 bits). */
static const uint16_t patterns[] = {
    0x0000, /* 0000 0000 0000 0000: all zeros                 */
    0xFFFF, /* 1111 1111 1111 1111: all ones                  */
    0xAAAA, /* 1010 1010 1010 1010: alternating, starts with 1 */
    0x5555, /* 0101 0101 0101 0101: alternating, starts with 0 */
    0x8001, /* 1000 0000 0000 0001: only the two end bits      */
    0x1234, /* 0001 0010 0011 0100: an irregular mix           */
};
#define NUM_PATTERNS (sizeof patterns / sizeof patterns[0])

static void test_Not(void) {
  for (bit in = 0; in <= 1; in++) {
    char what[32];
    snprintf(what, sizeof what, "Not(in=%d)", in);
    expect(what, Not(in), !in);
  }
}

static void test_And(void) {
  for (bit a = 0; a <= 1; a++) {
    for (bit b = 0; b <= 1; b++) {
      char what[32];
      snprintf(what, sizeof what, "And(a=%d, b=%d)", a, b);
      expect(what, And(a, b), a && b);
    }
  }
}

static void test_Or(void) {
  for (bit a = 0; a <= 1; a++) {
    for (bit b = 0; b <= 1; b++) {
      char what[32];
      snprintf(what, sizeof what, "Or(a=%d, b=%d)", a, b);
      expect(what, Or(a, b), a || b);
    }
  }
}

static void test_Xor(void) {
  for (bit a = 0; a <= 1; a++) {
    for (bit b = 0; b <= 1; b++) {
      char what[32];
      snprintf(what, sizeof what, "Xor(a=%d, b=%d)", a, b);
      expect(what, Xor(a, b), a ^ b);
    }
  }
}

static void test_Mux(void) {
  for (bit a = 0; a <= 1; a++) {
    for (bit b = 0; b <= 1; b++) {
      for (bit sel = 0; sel <= 1; sel++) {
        char what[32];
        snprintf(what, sizeof what, "Mux(a=%d, b=%d, sel=%d)", a, b, sel);
        expect(what, Mux(a, b, sel), sel ? b : a);
      }
    }
  }
}

static void test_DMux(void) {
  for (bit in = 0; in <= 1; in++) {
    for (bit sel = 0; sel <= 1; sel++) {
      bit a, b;
      DMux(in, sel, &a, &b);

      char what[32];
      snprintf(what, sizeof what, "DMux(in=%d, sel=%d).a", in, sel);
      expect(what, a, sel ? 0 : in);
      snprintf(what, sizeof what, "DMux(in=%d, sel=%d).b", in, sel);
      expect(what, b, sel ? in : 0);
    }
  }
}

static void test_Not16(void) {
  for (size_t p = 0; p < NUM_PATTERNS; p++) {
    bit in[16], out[16];
    to_bus(patterns[p], in);
    Not16(in, out);

    for (int i = 0; i < 16; i++) {
      char what[32];
      snprintf(what, sizeof what, "Not16(in=0x%04X)[%d]", patterns[p], i);
      expect(what, out[i], !in[i]);
    }
  }
}

static void test_And16(void) {
  for (size_t p = 0; p < NUM_PATTERNS; p++) {
    for (size_t q = 0; q < NUM_PATTERNS; q++) {
      bit a[16], b[16], out[16];
      to_bus(patterns[p], a);
      to_bus(patterns[q], b);
      And16(a, b, out);

      for (int i = 0; i < 16; i++) {
        char what[40];
        snprintf(what, sizeof what, "And16(a=0x%04X, b=0x%04X)[%d]",
                 patterns[p], patterns[q], i);
        expect(what, out[i], a[i] && b[i]);
      }
    }
  }
}

static void test_Or16(void) {
  for (size_t p = 0; p < NUM_PATTERNS; p++) {
    for (size_t q = 0; q < NUM_PATTERNS; q++) {
      bit a[16], b[16], out[16];
      to_bus(patterns[p], a);
      to_bus(patterns[q], b);
      Or16(a, b, out);

      for (int i = 0; i < 16; i++) {
        char what[40];
        snprintf(what, sizeof what, "Or16(a=0x%04X, b=0x%04X)[%d]", patterns[p],
                 patterns[q], i);
        expect(what, out[i], a[i] || b[i]);
      }
    }
  }
}

static void test_Mux16(void) {
  for (size_t p = 0; p < NUM_PATTERNS; p++) {
    for (size_t q = 0; q < NUM_PATTERNS; q++) {
      for (bit sel = 0; sel <= 1; sel++) {
        bit a[16], b[16], out[16];
        to_bus(patterns[p], a);
        to_bus(patterns[q], b);
        Mux16(a, b, sel, out);

        for (int i = 0; i < 16; i++) {
          char what[48];
          snprintf(what, sizeof what, "Mux16(a=0x%04X, b=0x%04X, sel=%d)[%d]",
                   patterns[p], patterns[q], sel, i);
          expect(what, out[i], sel ? b[i] : a[i]);
        }
      }
    }
  }
}

static void test_Or8Way(void) {
  /* Only 8 inputs, so test all 2^8 = 256 of them. */
  for (unsigned v = 0; v < 256; v++) {
    bit in[8];
    for (int i = 0; i < 8; i++) {
      in[i] = (v >> i) & 1;
    }

    char what[32];
    snprintf(what, sizeof what, "Or8Way(in=0x%02X)", v);
    expect(what, Or8Way(in), v != 0);
  }
}

static void test_Mux4Way16(void) {
  for (size_t pa = 0; pa < NUM_PATTERNS; pa++) {
    for (size_t pb = 0; pb < NUM_PATTERNS; pb++) {
      for (size_t pc = 0; pc < NUM_PATTERNS; pc++) {
        for (size_t pd = 0; pd < NUM_PATTERNS; pd++) {
          for (unsigned s = 0; s < 4; s++) {
            bit a[16], b[16], c[16], d[16], sel[2], out[16];
            to_bus(patterns[pa], a);
            to_bus(patterns[pb], b);
            to_bus(patterns[pc], c);
            to_bus(patterns[pd], d);
            sel[0] = s & 1;
            sel[1] = (s >> 1) & 1;
            Mux4Way16(a, b, c, d, sel, out);

            /* Reference: the input whose number equals sel. */
            const bit *inputs[4] = {a, b, c, d};
            const bit *expected = inputs[s];

            for (int i = 0; i < 16; i++) {
              char what[80];
              snprintf(what, sizeof what,
                       "Mux4Way16(a=0x%04X, b=0x%04X, c=0x%04X, d=0x%04X, "
                       "sel=%u)[%d]",
                       patterns[pa], patterns[pb], patterns[pc], patterns[pd],
                       s, i);
              expect(what, out[i], expected[i]);
            }
          }
        }
      }
    }
  }
}

static void test_Mux8Way16(void) {
  /* 8 inputs x all patterns would be ~215 million checks, so instead give
   * each input its own DIFFERENT pattern: if the chip picks the wrong
   * input, at least one wire is wrong. Then rotate the patterns so every
   * input also gets tried with every pattern. */
  static const uint16_t distinct[8] = {0x0000, 0xFFFF, 0xAAAA, 0x5555,
                                       0x00FF, 0xFF00, 0x0F0F, 0xF0F0};

  for (int r = 0; r < 8; r++) {
    bit in[8][16]; /* in[0] is input a, in[1] is b, ..., in[7] is h */
    for (int j = 0; j < 8; j++) {
      to_bus(distinct[(j + r) % 8], in[j]);
    }

    for (unsigned s = 0; s < 8; s++) {
      bit sel[3], out[16];
      sel[0] = s & 1;
      sel[1] = (s >> 1) & 1;
      sel[2] = (s >> 2) & 1;
      Mux8Way16(in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7], sel,
                out);

      for (int i = 0; i < 16; i++) {
        char what[48];
        snprintf(what, sizeof what, "Mux8Way16(rotation=%d, sel=%u)[%d]", r, s,
                 i);
        expect(what, out[i], in[s][i]);
      }
    }
  }
}

static void test_DMux4Way(void) {
  for (bit in = 0; in <= 1; in++) {
    for (unsigned s = 0; s < 4; s++) {
      bit sel[2], out[4]; /* out[0] is output a, ..., out[3] is d */
      sel[0] = s & 1;
      sel[1] = (s >> 1) & 1;
      DMux4Way(in, sel, &out[0], &out[1], &out[2], &out[3]);

      /* Reference: the output whose number equals sel gets in; the rest 0. */
      for (unsigned k = 0; k < 4; k++) {
        char what[40];
        snprintf(what, sizeof what, "DMux4Way(in=%d, sel=%u).%c", in, s,
                 "abcd"[k]);
        expect(what, out[k], k == s ? in : 0);
      }
    }
  }
}

int main(void) {
  test_Not();
  test_And();
  test_Or();
  test_Xor();
  test_Mux();
  test_DMux();
  test_Not16();
  test_And16();
  test_Or16();
  test_Mux16();
  test_Or8Way();
  test_Mux4Way16();
  test_Mux8Way16();
  test_DMux4Way();

  printf("%d checks, %d failures\n", checks, failures);
  return failures == 0 ? 0 : 1;
}
