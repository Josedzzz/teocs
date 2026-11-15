/*
 * Tests for the project 01 chips.
 *
 * Each test runs a chip over its whole truth table and compares the
 * result with a reference value computed with plain C operators.
 * (C operators are fine HERE: this is test code, not chip code.)
 */
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

int main(void) {
  test_Not();
  test_And();
  test_Or();
  test_Xor();
  test_Mux();
  test_DMux();

  printf("%d checks, %d failures\n", checks, failures);
  return failures == 0 ? 0 : 1;
}
