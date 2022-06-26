#ifndef NSH_DIAGNOSIS_H
#define NSH_DIAGNOSIS_H

#include "nsh/common.h"
#include "nsh/location.h"

struct diagnosis {
  enum {
    diagnosis_info,
    diagnosis_warn,
    diagnosis_error,
  } level;
  enum {
    diagnosis_unexpected,
    diagnosis_overflow_i,
    diagnosis_overflow_u,
    diagnosis_invalid_digit,
    diagnosis_invalid_suffix,
    diagnosis_no_memory,
  } type;
  struct location location;
  union {
    struct {
      char const *expected;
      char const *got;
    } unexpect;
    struct {
      char const *literal;
      union {
        union {
          ssize_t i;
          size_t u;
        } truncated;
        size_t limit;
      };
    } overflow;
    struct {
      unsigned char base;
      char digit;
    } invalid_digit;
  };
};

#endif /* NSH_DIAGNOSIS_H */
