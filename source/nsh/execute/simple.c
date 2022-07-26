#include "nsh/execute.h"
#include "nsh/context.h"

#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>

bool execute_simple(struct stack *job, struct simple simple,
                    struct redirect redirect, struct pipe *pipe) {
  if (simple.argument.size == 0) {
    todo("Handling redirect and variable assignment when no command is given");
  } else {
    struct program program;
    program_init(&program);
    bool ok =
      setup_program(&program, simple) &&
      execute_program(job, program, redirect, pipe);
    program_destroy(&program);
    return ok;
  }
}
