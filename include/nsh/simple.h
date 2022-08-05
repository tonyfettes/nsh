#ifndef NSH_SIMPLE_H
#define NSH_SIMPLE_H

#include "nsh/stack.h"
#include "nsh/word.h"
#include "nsh/environment.h"
#include "nsh/redirect.h"
#include "nsh/file.h"

struct simple {
  bool expanded;
  // Stack of `struct word`, not expanded, or stack of `struct string`,
  // expanded.
  struct stack argument;
  union {
    // Stack of `struct assignment`.
    struct stack assignment;
    struct stack environment;
  };
};

void simple_init(struct simple *simple);

void simple_clear(struct simple *simple);

void simple_destroy(struct simple *simple);

bool simple_expand(struct simple simple, struct simple *target);

bool simple_execute(struct simple simple, struct context *context,
                    struct stack redirect, struct pipe *pipe);

bool simple_display(struct simple simple, struct string *target);

#endif // NSH_SIMPLE_H
