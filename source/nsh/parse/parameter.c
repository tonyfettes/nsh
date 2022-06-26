#include "nsh/parse/parameter.h"
#include "nsh/parse/name.h"
#include "nsh/parse/word.h"

#include <string.h>

bool parse_parameter(struct parse *restrict parse,
                     struct parameter *restrict parameter) {
  char c;
  try(parse_peek(parse, &c));
  if (c == '#') {
    parameter->type = parameter_length;
    try(parse_bump(parse));
  } else {
    parameter->type = parameter_simple;
  }
  try(parse_name(parse, &parameter->name));
  try(parse_peek(parse, &c));
  if (parameter->type == parameter_length) {
    if (c != '}') {
      stack_push(&parse->diagnosis, &(struct diagnosis) {
        .level = diagnosis_error,
        .type = diagnosis_unexpected,
        .location = parse->location,
        .unexpect = {
          .expected = "'}'",
        },
      }, sizeof(struct diagnosis));
      return false;
    }
    return parse_bump(parse);
  }
  parameter->substitute_null = false;
  if (c == ':') {
    parameter->substitute_null = true;
    try(parse_bump_peek(parse, &c));
  }
  switch (c) {
  case '#':
  case '%':
    if (parameter->substitute_null) {
      try(stack_push(&parse->diagnosis, &(struct diagnosis) {
        .level = diagnosis_error,
        // TODO:
      }, sizeof(struct diagnosis)));
      return false;
    }
    parameter->type = c;
    try(parse_bump(parse));
    char p = c;
    try(parse_peek(parse, &c));
    if (p == c) {
      parameter->longest_match = true;
      try(parse_bump_peek(parse, &c));
    }
    break;
  case '-':
  case '=':
  case '?':
  case '+':
    parameter->type = c;
    try(parse_bump_peek(parse, &c));
    break;
  case '}':
    try(parse_bump(parse));
    return true;
  default:
    try(stack_push(&parse->diagnosis, &(struct diagnosis) {
      .level = diagnosis_error,
      .type = diagnosis_unexpected,
      .location = parse->location,
      .unexpect = {
        .expected = "':', '-', '=', '?', '+', '#', or '%'",
      },
    }, sizeof(struct diagnosis)));
    return false;
    break;
  }
  try(parse_word(parse, &parameter->word));
  try(parse_peek(parse, &c));
  if (c != '}') {
    // TODO:
    return false;
  }
  return parse_bump(parse);
}
