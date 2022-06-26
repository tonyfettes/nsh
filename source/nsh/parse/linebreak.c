#include "linebreak.h"

#include <ctype.h>

extern bool parse_linebreak(struct context *restrict context) {
  char c = peek(context);
  while (!isspace(c)) {
    return false;
  }
  do {
    context->cptr++;
    c = peek(context);
  } while (isspace(c));
  return true;
}
