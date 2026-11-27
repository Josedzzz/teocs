/*
 * Tests for the project 02 chips.
 *
 * Each test runs a chip over its inputs and compares the result with a
 * reference value computed with plain C arithmetic.
 * (C operators are fine HERE: this is test code, not chip code.)
 */
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

int main(void) {
  test_HalfAdder();

  printf("%d checks, %d failures\n", checks, failures);
  return failures == 0 ? 0 : 1;
}
