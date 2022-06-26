#include "nsh/parse/loop.h"

#include "nsh/parse/block.h"
#include "nsh/parse/delimiter.h"
#include "nsh/parse/name.h"

bool parse_while_loop(struct parse *parse, struct while_loop *while_loop) {
  try(parse_block(parse, &while_loop->condition, keyword_do));
  try(parse_linebreak(parse));
  try(parse_block(parse, &while_loop->body, keyword_done));
  return true;
}

bool parse_until_loop(struct parse *parse, struct until_loop *until_loop) {
  try(parse_block(parse, &until_loop->condition, keyword_do));
  try(parse_linebreak(parse));
  try(parse_block(parse, &until_loop->body, keyword_done));
  return true;
}

bool parse_for_loop(struct parse *parse, struct for_loop *for_loop) {
  try(parse_name(parse, &for_loop->name));
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
    try(parse_block(parse, &for_loop->body, keyword_done));
    return true;
  }
  if (c == ';') {
    try(parse_bump(parse));
    try(parse_linebreak(parse));
    // try(parse_expect(parse, "do"));
    try(parse_linebreak(parse));
    try(parse_block(parse, &for_loop->body, keyword_done));
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
    try(parse_block(parse, &for_loop->body, keyword_done));
    return true;
  }
  return false;
}
