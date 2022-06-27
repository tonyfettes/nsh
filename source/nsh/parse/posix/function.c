#include "nsh/parse/posix/function.h"

bool parse_function(struct parse *parse, struct function *function,
                    struct stack *redirect) {
  char c;
  try(parse_peek(parse, &c));
  assert(!isblank(c));
}
