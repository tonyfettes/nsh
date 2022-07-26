#ifndef NSH_MATH_H
#define NSH_MATH_H

#include "nsh/common.h"

int int_min(int a, int b);
int int_max(int a, int b);

unsigned int unsigned_int_min(unsigned int a, unsigned int b);
unsigned int unsigned_int_max(unsigned int a, unsigned int b);

unsigned long unsigned_long_min(unsigned long a, unsigned long b);
unsigned long unsigned_long_max(unsigned long a, unsigned long b);

unsigned long long unsigned_long_long_min(unsigned long long a,
                                          unsigned long long b);
unsigned long long unsigned_long_long_max(unsigned long long a,
                                          unsigned long long b);

#define min(a, b)                                                      \
  _Generic((a),                                                        \
    int: int_min,                                                      \
    unsigned int: unsigned_int_min,                                    \
    unsigned long: unsigned_long_min,                                  \
    unsigned long long: unsigned_long_long_min                         \
  )(a, b)

#define max(a, b)                                                      \
  _Generic((a),                                                        \
    int: int_max,                                                      \
    unsigned int: unsigned_int_min,                                    \
    unsigned long: unsigned_long_max,                                  \
    unsigned long long: unsigned_long_long_max                         \
  )(a, b)

#endif // NSH_MATH_H
