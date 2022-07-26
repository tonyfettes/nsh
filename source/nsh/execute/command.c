#include "nsh/execute/command.h"
#include "nsh/execute/simple.h"
#include "nsh/function.h"
#include "nsh/context.h"
#include "nsh/process.h"

bool execute_command(struct stack *job, struct command command,
                     struct pipe pipe) {
  switch (command.type) {
    struct process *proc;
  case command_none:
  case command_keyword:
    break;
  case command_simple:
    break;
  case command_brace:
    break;
  case command_subshell:
    break;
  case command_loop:
    break;
  case command_for_in:
    break;
  case command_function:
    function_table_insert(&context.function_table, *command.function);
    break;
  }
  return true;
}
