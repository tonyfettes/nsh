#ifndef NSH_WORD_H
#define NSH_WORD_H

#include "nsh/common.h"
#include "nsh/stack.h"
#include "nsh/string.h"

struct word {
  enum word_type {
    word_segment,
    word_plain,
    word_field,
  } type;
  bool quoted;
  union {
    // Stack of `struct segment`.
    struct stack segment;
    struct string plain;
    // Stack of `struct string`.
    struct stack field;
  };
};

void word_init(struct word *word);

void word_select(struct word *word, enum word_type type);

void word_clear(struct word *word);

void word_destroy(struct word *word);

bool word_expand(struct word *word);

struct segment;

bool word_push(struct word *word, struct segment *segment);

bool word_putc(struct word *word, char ch);

bool word_puts(struct word *word, struct string *string);

#endif // NSH_WORD_H
