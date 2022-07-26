#ifndef NSH_SIMPLE_H
#define NSH_SIMPLE_H

#include "nsh/stack.h"
#include "nsh/word.h"
#include "nsh/environment.h"
#include "nsh/redirect.h"

struct simple {
  // Stack of `struct word`, not expanded.
  struct stack argument;
  // Stack of `struct assignment`.
  struct stack assignment;
};

void simple_init(struct simple *simple);

void simple_clear(struct simple *simple);

void simple_destroy(struct simple *simple);

bool simple_display(struct simple simple, struct string *target);

#endif // NSH_SIMPLE_H
