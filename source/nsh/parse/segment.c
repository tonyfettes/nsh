#include "nsh/parse/segment.h"

#include "nsh/parse/parameter.h"
#include "nsh/parse/name.h"
#include "nsh/parse/command.h"
#include "nsh/parse/delimiter.h"
#include "nsh/assert.h"

#include <ctype.h>

bool is_expansion(char ch) {
  return ch == '{' || ch == '(' || isdigit(ch)
         || is_special_parameter(ch);
}

bool parse_segment(struct parse *parse, struct segment *segment) {
  char c;
  try(parse_peek(parse, &c));
  switch (c) {
  case '{':
    try(parse_bump(parse));
    parse->parameter_level++;
    segment_select(segment, segment_parameter);
    try(parse_parameter(parse, &segment->parameter));
    break;
  case '(':
    try(parse_bump_peek(parse, &c));
    if (c == '(') {
      todo("Arithmetic expansion");
    } else {
      segment_select(segment, segment_command);
      try(parse_command(parse, &segment->command));
      try(parse_blank(parse));
      try(parse_peek(parse, &c));
      if (c != ')') {
        try(stack_push(&parse->diagnosis, &(struct diagnosis) {
          .level = diagnosis_error,
          .type = diagnosis_unexpected,
          .location = parse->location,
          .unexpect = {
            .expected = ")",
          },
        }, sizeof(struct diagnosis)));
        return false;
      }
      return true;
    }
    break;
  default:
    segment_select(segment, segment_parameter);
    if (isdigit(c) || is_special_parameter(c)) {
      try(string_putc(&segment->parameter.name, c));
    } else {
      try(parse_name(parse, &segment->parameter.name));
    }
  }
  return true;
}
