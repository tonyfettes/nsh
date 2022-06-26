#include "nsh/parse/environment.h"

#include "nsh/parse/name.h"
#include "nsh/parse/word.h"
#include "nsh/parse/segment.h"

#include <ctype.h>

bool parse_environment(struct parse *restrict parse,
                       struct environment *restrict env) {
  char c;
  try(parse_peek(parse, &c));
  if (c == '_' || isalpha(c)) {
    try(parse_name(parse, &env->name));
    if (c != '=') {
      // Move `name` into `value`.
      try(word_push(&env->value, &(struct segment) {
        .type = segment_plain,
        .plain = env->name,
      }));
      // Re-init the `name` string, so that caller could detect that it
      // is not an environment assignment.
      string_init(&env->name);
    } else {
      // Skip '='.
      try(parse_bump(parse));
    }
  }
  return parse_word(parse, &env->value);
}
