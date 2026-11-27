/*
 * Tests for the project 02 chips.
 *
 * Each test runs a chip over its inputs and compares the result with a
 * reference value computed with plain C arithmetic.
 * (C operators are fine HERE: this is test code, not chip code.)
 */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "arith.h"

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

/* 16-bit inputs chosen for arithmetic: edge values and long carry chains. */
static const uint16_t patterns[] = {
    0x0000, 0x0001, 0xFFFF, 0x7FFF, 0x8000, 0xAAAA, 0x5555, 0x1234,
};
#define NUM_PATTERNS (sizeof patterns / sizeof patterns[0])

static void test_HalfAdder(void) {
  for (bit a = 0; a <= 1; a++) {
    for (bit b = 0; b <= 1; b++) {
      bit sum, carry;
      HalfAdder(a, b, &sum, &carry);

      /* Reference: a + b is 0, 1 or 2 (binary 00, 01, 10). */
      int total = a + b;
      char what[40];
      snprintf(what, sizeof what, "HalfAdder(a=%d, b=%d).sum", a, b);
      expect(what, sum, total & 1);
      snprintf(what, sizeof what, "HalfAdder(a=%d, b=%d).carry", a, b);
      expect(what, carry, total >> 1);
    }
  }
}

static void test_FullAdder(void) {
  for (bit a = 0; a <= 1; a++) {
    for (bit b = 0; b <= 1; b++) {
      for (bit c = 0; c <= 1; c++) {
        bit sum, carry;
        FullAdder(a, b, c, &sum, &carry);

        /* Reference: a + b + c is 0..3 (binary 00, 01, 10, 11). */
        int total = a + b + c;
        char what[48];
        snprintf(what, sizeof what, "FullAdder(a=%d, b=%d, c=%d).sum", a, b, c);
        expect(what, sum, total & 1);
        snprintf(what, sizeof what, "FullAdder(a=%d, b=%d, c=%d).carry", a, b,
                 c);
        expect(what, carry, total >> 1);
      }
    }
  }
}

static void test_Add16(void) {
  for (size_t p = 0; p < NUM_PATTERNS; p++) {
    for (size_t q = 0; q < NUM_PATTERNS; q++) {
      bit a[16], b[16], out[16];
      to_bus(patterns[p], a);
      to_bus(patterns[q], b);
      Add16(a, b, out);

      /* Reference: real addition, cut to 16 bits (the overflow carry is
       * dropped, like in the chip). */
      bit expected[16];
      to_bus((uint16_t)(patterns[p] + patterns[q]), expected);

      for (int i = 0; i < 16; i++) {
        char what[40];
        snprintf(what, sizeof what, "Add16(a=0x%04X, b=0x%04X)[%d]",
                 patterns[p], patterns[q], i);
        expect(what, out[i], expected[i]);
      }
    }
  }
}

static void test_Inc16(void) {
  for (size_t p = 0; p < NUM_PATTERNS; p++) {
    bit in[16], out[16];
    to_bus(patterns[p], in);
    Inc16(in, out);

    /* Reference: real +1, cut to 16 bits (0xFFFF + 1 wraps to 0x0000). */
    bit expected[16];
    to_bus((uint16_t)(patterns[p] + 1), expected);

    for (int i = 0; i < 16; i++) {
      char what[32];
      snprintf(what, sizeof what, "Inc16(in=0x%04X)[%d]", patterns[p], i);
      expect(what, out[i], expected[i]);
    }
  }
}

int main(void) {
  test_HalfAdder();
  test_FullAdder();
  test_Add16();
  test_Inc16();

  printf("%d checks, %d failures\n", checks, failures);
  return failures == 0 ? 0 : 1;
}
