#ifndef NSH_SIMPLE_H
#define NSH_SIMPLE_H

#include "nsh/stack.h"
#include "nsh/word.h"
#include "nsh/environment.h"

struct simple {
  // Stack of `struct word`, not expanded.
  struct stack argument;
  // Stack of `struct environment`.
  struct stack environment;
};

void simple_init(struct simple *simple);

void simple_clear(struct simple *simple);

void simple_expand(struct simple *simple);

void simple_destroy(struct simple *simple);

#endif // NSH_SIMPLE_H
