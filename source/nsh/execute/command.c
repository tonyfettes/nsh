#include "nsh/execute/command.h"
#include "nsh/execute/simple.h"
#include "nsh/function.h"
#include "nsh/context.h"

bool execute_command(struct command *command) {
  switch (command->type) {
  case command_none:
  case command_keyword:
    break;
  case command_simple:
    return execute_simple(&command->simple, &command->redirect);
    break;
  case command_brace:
    break;
  case command_subshell:
    break;
  case command_while_loop:
    break;
  case command_until_loop:
    break;
  case command_for_loop:
    break;
  case command_function:
    function_table_insert(&context.function_table, command->function);
    break;
  }
}
