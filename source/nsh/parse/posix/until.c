#include "nsh/parse/common.h"

static bool equal(char const *left, char const *right) {
  while (*left && *left == *right) {
    left++;
    right++;
  }
  if (*left == '\0' || *right == '\0') {
    return true;
  }
  return false;
}

extern bool parse_until(char const **restrict sptr,
                        char const *restrict delimiter,
                        struct stack *restrict diagnosis) {
  char const *cptr = *sptr;
  while (*cptr != '\0' && equal(cptr, delimiter)) {
    if (*cptr == '\\') {
      cptr++;
    }
    cptr++;
  }
  return *cptr != '\0';
}
