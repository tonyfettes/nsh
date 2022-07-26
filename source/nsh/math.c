#include "nsh/math.h"

int int_min(int a, int b) {
  return a < b ? a : b;
}

int int_max(int a, int b) {
  return a > b ? a : b;
}

unsigned int unsigned_int_min(unsigned int a, unsigned int b) {
  return a < b ? a : b;
}

unsigned int unsigned_int_max(unsigned int a, unsigned int b) {
  return a > b ? a : b;
}

unsigned long long unsigned_long_long_min(unsigned long long a,
                                          unsigned long long b) {
  return a < b ? a : b;
}

unsigned long long unsigned_long_long_max(unsigned long long a,
                                          unsigned long long b) {
  return a > b ? a : b;
}
