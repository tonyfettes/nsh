#ifndef NSH_PARSE_INTEGER_H
#define NSH_PARSE_INTEGER_H

#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include "stack.h"
#include "parse/expect.h"

enum {
  parse_oct = 1 << 0,
  parse_dec = 1 << 1,
  parse_hex = 1 << 2,
  parse_all = parse_oct | parse_dec | parse_hex,
};

extern bool parse_size_t(char const **restrict string,
                         size_t *restrict num, int flags,
                         struct stack *restrict diagnosis);

extern bool parse_ssize_t(char const **restrict string,
                          ssize_t *restrict num, int flags,
                          struct stack *restrict diagnosis);

#endif /* NSH_PARSE_INTEGER_H */
