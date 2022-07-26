#include "nsh/parse/posix/loop.h"

#include "nsh/parse/posix/block.h"
#include "nsh/parse/posix/delimiter.h"
#include "nsh/parse/posix/name.h"

bool parse_loop(struct parse *parse, struct loop *loop) {
  try(parse_block(parse, &loop->condition, keyword_do));
  try(parse_linebreak(parse));
  try(parse_block(parse, &loop->body, keyword_done));
  return true;
}

bool parse_for_in(struct parse *parse, struct for_in *for_in) {
  try(parse_name(parse, &for_in->name));
  try(parse_linebreak(parse));
  char c;
  try(parse_peek(parse, &c));
  if (c == 'd') {
    try(parse_bump_peek(parse, &c));
    if (c != 'o') {
      try(stack_push(&parse->diagnosis, &(struct diagnosis) {
        .level = diagnosis_error,
        .type = diagnosis_unexpected,
        .unexpect = {
          .expected = "do",
        }
      }, sizeof(struct diagnosis)));
      return false;
    }
    try(parse_bump(parse));
    try(parse_blank(parse));
    try(parse_block(parse, &for_in->body, keyword_done));
    return true;
  }
  if (c == ';') {
    try(parse_bump(parse));
    try(parse_linebreak(parse));
    // try(parse_expect(parse, "do"));
    try(parse_linebreak(parse));
    try(parse_block(parse, &for_in->body, keyword_done));
    return true;
  }
  if (c == 'i') {
    try(parse_bump_peek(parse, &c));
    if (c != 'n') {
      try(stack_push(&parse->diagnosis, &(struct diagnosis) {
        .level = diagnosis_error,
        .type = diagnosis_unexpected,
        .unexpect = {
          .expected = "in",
        }
      }, sizeof(struct diagnosis)));
      return false;
    }
    try(parse_blank(parse));
    try(parse_peek(parse, &c));
    if (c == ';') {
      try(parse_bump(parse));
    }
    try(parse_linebreak(parse));
    // try(parse_expect(parse, "do"));
    try(parse_block(parse, &for_in->body, keyword_done));
    return true;
  }
  return false;
}
