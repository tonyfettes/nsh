#ifndef NSH_CONTEXT_H
#define NSH_CONTEXT_H

#include "nsh/alias.h"
#include "nsh/function.h"
#include "nsh/variable.h"
#include "nsh/builtin.h"
#include "nsh/job.h"

struct context {
  struct alias_table alias_table;
  struct function_table function_table;
  struct hash variable;
  struct hash builtin;
  // Set of external executable program.
  struct hash program_table;
  struct stack job;
  // Past working directories.
  struct stack pwd;
  // Polling file descriptor. Stack of `int`.
  struct stack pfd;
};

extern struct context context;

void context_init(struct context *context);

void context_poll(struct context *context);

void context_destroy(struct context *context);

#endif // NSH_CONTEXT_H
