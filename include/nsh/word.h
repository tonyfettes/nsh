#ifndef NSH_WORD_H
#define NSH_WORD_H

#include "nsh/common.h"
#include "nsh/stack.h"
#include "nsh/string.h"
#include "nsh/parse.h"

struct word {
  bool quoted;
  // Stack of `struct segment`.
  struct stack segment;
};

void word_init(struct word *word);

void word_clear(struct word *word);

void word_destroy(struct word *word);

// Expand `word`, push results to the end of `target`.
bool word_expand(struct word word, struct string *target);

bool word_split(struct word word, struct stack *target,
                struct string delimiter);

struct segment;

bool word_push(struct word *word, struct segment *segment);

bool word_putc(struct word *word, char ch);

bool word_puts(struct word *word, struct string *string);

#endif // NSH_WORD_H
