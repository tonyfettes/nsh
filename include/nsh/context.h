#ifndef NSH_CONTEXT_H
#define NSH_CONTEXT_H

#include "nsh/alias.h"
#include "nsh/function.h"

struct context {
  struct alias_table alias_table;
  struct function_table function_table;
};

extern struct context context;

void context_init(struct context *context);

void context_destroy(struct context *context);

#endif // NSH_CONTEXT_H
