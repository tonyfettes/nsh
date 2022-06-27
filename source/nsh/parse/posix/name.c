#include "nsh/parse/posix/name.h"

#include <ctype.h>

bool parse_name(struct parse *parse, struct string *name) {
  char c;
  try(parse_peek(parse, &c));
  do {
    try(string_putc(name, c));
    try(parse_bump_peek(parse, &c));
  } while (c == '_' || isalpha(c));
  return true;
}
