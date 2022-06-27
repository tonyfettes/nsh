#include "nsh/parse/posix/word.h"

#include "nsh/parse/posix/operator.h"
#include "nsh/parse/posix/segment.h"
#include "nsh/parse/posix/command.h"
#include "nsh/parse/posix/name.h"
#include "nsh/parse/posix/delimiter.h"

static bool parse_single_quoted(struct parse *restrict parse,
                                struct word *restrict word) {
  char c;
  try(parse_peek(parse, &c));
  while (c != '\'') {
    try(word_putc(word, c));
    try(parse_bump_peek(parse, &c));
  }
  try(parse_bump(parse));
  return true;
}

static bool parse_double_quoted(struct parse *restrict parse,
                                struct word *restrict word,
                                char delimiter) {
  char c;
  try(parse_peek(parse, &c));
  struct segment segment;
  while (c != delimiter) {
    switch (c) {
    case '\\':
      try(parse_bump_peek(parse, &c));
      switch (c) {
      default:
        try(word_putc(word, '\\'));
        // fallthrough
      case '$':
      case '`':
      case '"':
        try(word_putc(word, c));
      case '\n':
        break;
      }
      break;
    case '`':
      segment_select(&segment, segment_command);
      try(parse_command(parse, &segment.command));
      try(word_push(word, &segment));
      break;
    case '$':
      try(parse_bump(parse));
      segment_init(&segment);
      if (!parse_segment(parse, &segment)) {
        segment_destroy(&segment);
        return false;
      }
      if (!word_push(word, &segment)) {
        segment_destroy(&segment);
        return false;
      }
      segment_destroy(&segment);
      break;
    default: 
      try(word_putc(word, c));
      break;
    }
    try(parse_bump_peek(parse, &c));
  }
  try(parse_bump(parse));
  return true;
}

bool parse_word_char(struct parse *parse, struct word *word,
                     struct segment *segment, char c) {
  switch (c) {
  case '\\':
    try(parse_bump_peek(parse, &c));
    if (c == '\n') {
      try(parse_bump_peek(parse, &c));
    } else {
      try(word_putc(word, c));
      word->quoted = true;
      try(parse_bump(parse));
    }
    break;
  case '\'':
    try(parse_bump(parse));
    try(parse_single_quoted(parse, word));
    word->quoted = true;
    break;
  case '"':
    try(parse_bump(parse));
    try(parse_double_quoted(parse, word, '"'));
    word->quoted = true;
    break;
  case '$':
    try(parse_bump(parse));
    try(parse_segment(parse, segment));
    try(word_push(word, segment));
    word->quoted = true;
    break;
  default:
    try(word_putc(word, c));
    try(parse_bump(parse));
    break;
  }
  return true;
}

bool parse_word(struct parse *parse, struct word *word) {
  char c;
  try(parse_peek(parse, &c));
  assert(!is_operator(c));
  struct segment segment;
  segment_init(&segment);
  bool ok = true;
  do {
    ok = parse_word_char(parse, word, &segment, c);
    if (!ok) {
      break;
    }
    parse_peek(parse, &c);
  } while (!is_delimiter(parse, c));
  segment_destroy(&segment);
  return true;
}
