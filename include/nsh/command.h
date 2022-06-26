#ifndef NSH_COMMAND_H
#define NSH_COMMAND_H

#include "nsh/simple.h"
#include "nsh/keyword.h"
#include "nsh/block.h"
#include "nsh/loop.h"
#include "nsh/compound.h"
#include "nsh/redirect.h"

struct command {
  enum command_type {
    command_none,
    command_keyword,
    command_simple,
    command_brace,
    command_subshell,
    command_while_loop,
    command_until_loop,
    command_for_loop,
    command_function,
  } type;
  union {
    enum keyword keyword;
    struct simple simple;
    struct brace brace;
    struct subshell subshell;
    struct while_loop while_loop;
    struct until_loop until_loop;
    struct for_loop for_loop;
    struct function *function;
  };
  // Stack of `struct redirect`.
  struct stack redirect;
};

void command_init(struct command *command);

void command_clear(struct command *command);

bool command_select(struct command *command, enum command_type type);

void command_destroy(struct command *command);

#endif // NSH_COMMAND_H
