#ifndef NSH_INVOCATION_H
#define NSH_INVOCATION_H

#include "nsh/stack.h"

struct invocation {
  // Stack of `struct string`, expanded.
  struct stack argument;
  // Stack of `struct environment`.
  struct stack environment;
  // Stack of `struct redirect`.
  struct stack redirect;
};

void invocation_init(struct invocation *invocation);

void invocation_clear(struct invocation *invocation);

void invocation_destroy(struct invocation *invocation);

#endif // NSH_INVOCATION_H
