#include "nsh/execute.h"
#include "nsh/context.h"

static struct string const ifs_key = {
  .capacity = 4,
  .size = 3,
  .stack = "IFS",
};

static bool setup_program(struct program *program,
                          struct simple simple) {
  struct string ifs;
  string_init(&ifs);
  hash_size_t index;
  if (!hash_find(context.variable, ifs_key, &index)) {
    string_puts(&ifs, " \t\n\r");
  }
  bool ok = true;
  for (struct word *word = stack_head(&simple.argument, 0);
       word != stack_tail(&simple.argument, 0); word++) {
    if (!word_split(*word, &program->argument, ifs)) {
      ok = false;
      goto release_ifs;
    }
  }

  for (struct assignment *assign = stack_head(&simple.assignment, 0);
       assign != stack_tail(&simple.assignment, 0); assign++) {
    if (!stack_alloc(&program->environment,
                     sizeof(struct environment))) {
      ok = false;
      goto release_ifs;
    }
    struct environment *envi = stack_tail(&program->environment, 0);
    environment_init(envi);
    if (!word_expand(assign->value, &envi->value)) {
      environment_destroy(envi);
      ok = false;
      goto release_ifs;
    }
    assert(stack_bump(&program->environment,
                      sizeof(struct environment)));
  }

release_ifs:
  string_destroy(&ifs);
  return ok;
}

enum invoke_flags {
  invoke_subshell = 1 << 0,
};

struct builtin_param {
  struct builtin builtin;
  struct stack argument;
  struct stack environment;
  struct file_stdio stdio;
};

static bool builtin_wrapper(void *data) {
  struct builtin_param *param = data;
  return param->builtin.main(param->argument, param->environment,
                             param->stdio);
}

static bool invoke(struct job *job, struct invocation invocation,
                   struct pipe *pipe, unsigned int flags) {
  struct string *name = stack_head(&invocation.argument, 0);
  hash_size_t index;
  if (hash_find(context.builtin, *name, &index)) {
    struct builtin *builtin = hash_head(&context.builtin, index, 0);
    if (flags & invoke_subshell) {
      struct builtin_param param = {
        .builtin = *builtin,
        .argument = invocation.argument,
        .environment = invocation.environment,
        .stdio = file_stdio,
      };
      if (!group_fork(&job->group, builtin_wrapper, &param,
                      invocation.redirect, pipe)) {
        return false;
      }
    } else {
      for (struct redirect *redirect = stack_head(&invocation.redirect, 0);
           redirect != stack_tail(&invocation.redirect, 0); redirect++) {
        switch (redirect->type) {
        case redirect_descriptor:
          break;
        case redirect_path:
          break;
        case redirect_heredoc:
          break;
        }
      }
      // builtin->main(invocation.argument, invocation.environment
    }
    todo("Execute builtin");
  } else {
    struct string path;
    string_init(&path);
    if (!group_spawn(&job->group, path, invocation.argument,
                     invocation.environment, invocation.redirect,
                     pipe)) {
      return false;
    }
  }
  return true;
}

static bool execute_program(struct job *job, struct program program,
                            struct stack redirect, struct pipe *pipe) {
  struct string *name = stack_head(&program.argument, 0);
  hash_size_t index;
  if (hash_find(context.builtin, *name, &index)) {
    todo("Execute builtin");
  } else {
    if (!group_spawn(&job->group, program, redirect, pipe)) {
      return false;
    }
  }
  return true;
}

bool execute_pipeline(struct stack *job_list, struct pipeline pipeline,
                      struct pipe *pipe) {
  stack_size_t pipeline_size;
  pipeline_size = pipeline.command.size / sizeof(struct pipeline);
  assert(pipeline_size != 0);
  if (pipeline_size == 1) {
    struct command *command = stack_head(&pipeline.command, 0);
    switch (command->type) {
    case command_none:
    case command_keyword:
      break;
    case command_simple:
      if (command->simple.argument.size == 0) {
      }
      break;
    case command_brace:
    case command_subshell:
    case command_loop:
    case command_for_in:
    case command_function:
      break;
    }
  }
  if (!stack_alloc(job_list, sizeof(struct job))) {
    return false;
  }
  struct job *job = stack_tail(job_list, 0);
  job_init(job);
  struct command *head = stack_head(&pipeline.command, 0);
  struct command *tail = stack_tail(&pipeline.command, 0);
  struct file last = { -1 };
  for (struct command *command = head; command != tail; command++) {
    switch (command->type) {
    case command_keyword:
    case command_none:
      assert(command->type != command_keyword &&
             command->type != command_none);
      break;
    case command_simple:
      if (command->simple.argument.size == 0) {
        todo("Handling redirect and variable assignment when no command is given");
      } else {
        struct program program;
        program_init(&program);
        bool ok =
          setup_program(&program, command->simple) &&
          execute_program(job, program, &last, command->redirect);
        program_destroy(&program);
        return ok;
      }
      break;
    case command_for_in:
      // for (...) {
      //   ...
      // }
      break;
    case command_brace:
      break;
    case command_subshell:
      break;
    case command_loop:
      break;
    case command_function:
      break;
    };
  }
  return true;
}
