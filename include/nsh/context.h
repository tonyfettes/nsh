#ifndef NSH_CONTEXT_H
#define NSH_CONTEXT_H

#include "nsh/variable.h"
#include "nsh/builtin.h"
#include "nsh/function.h"
#include "nsh/job.h"
#include "nsh/option.h"
#include <sys/poll.h>
#include <unistd.h>

struct file_relocation {
  union {
    struct stack stack;
    struct {
      stack_size_t capactiy;
      int descriptor[3];
    };
  };
};

// A shell execution environment consists of the following:
// - Open files inherited upon invocation of the shell, plus open files controlled by exec
// - Working directory as set by cd
// - File creation mask set by umask
// - [XSI] [Option Start] File size limit as set by ulimit [Option End]
// - Current traps set by trap
// - Shell parameters that are set by variable assignment (see the set special built-in) or from the System Interfaces volume of POSIX.1-2017 environment inherited by the shell when it begins (see the export special built-in)
// - Shell functions; see Function Definition Command
// - Options turned on at invocation or by set
// - Process IDs of the last commands in asynchronous lists known to this shell environment; see Asynchronous Lists
// - Shell aliases; see Alias Substitution
struct context {
  mode_t umask;
  struct option option;
  // Hash map from `struct string` to `struct string`.
  struct hash alias;
  struct hash variable;
  struct hash builtin;
  struct hash function;
  struct stack job;
  // Stack of `struct command` that to are still in execution.
  struct stack command;
  // Stack of `struct string`, past and current working directory.
  struct stack directory;
  // Stack of `struct descriptor`.
  struct stack descriptor;
  // Stack of `struct block`.
  struct stack block;
};

bool context_init(struct context *context);

bool context_fork(struct context *target, struct context source);

bool context_destroy(struct context *context, struct context source);

#endif // NSH_CONTEXT_H
