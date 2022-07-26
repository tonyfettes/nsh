#include "nsh/parse/posix/delimiter.h"

#include "nsh/parse/posix/operator.h"

#include <ctype.h>

bool is_delimiter(struct parse *parse, char ch) {
  return is_operator(ch) ||
         isblank(ch) || 
         ch == '`' ||
         (parse->parameter_level && ch == '}');
}

bool parse_delimiter(struct parse *restrict parse) {
  char c;
  parse_peek(parse, &c);
  if (!is_delimiter(parse, c)) {
    stack_push(&parse->diagnosis, &(struct diagnosis) {
      .level = diagnosis_error,
      .type = diagnosis_unexpected,
      .unexpect = {
        .expected = "delimiter",
      },
    }, sizeof(struct diagnosis));
    return false;
  }
  while (isblank(c)) {
    if (!parse_bump(parse)) {
      return false;
    }
    if (!parse_peek(parse, &c)) {
      return false;
    }
  }
  return true;
}

bool parse_blank(struct parse *parse) {
  char c;
  parse_peek(parse, &c);
  while (isblank(c)) {
    try(parse_bump_peek(parse, &c));
  }
  return true;
}

bool parse_linebreak(struct parse *parse) {
  char c;
  try(parse_peek(parse, &c));
  if (isblank(c)) {
    try(parse_blank(parse));
  }
  while (c == '\n') {
    try(parse_bump_peek(parse, &c));
    if (isblank(c)) {
      try(parse_blank(parse));
      continue;
    }
  }
  return true;
}
