#include "nsh/command.h"

#include "nsh/function.h"
#include "nsh/context.h"
#include "nsh/block.h"

static void command_main_destroy(struct command *command) {
  switch (command->type) {
  case command_none:
  case command_keyword:
    break;
  case command_simple:
    simple_destroy(&command->simple);
    break;
  case command_brace:
    brace_destroy(&command->brace);
    break;
  case command_subshell:
    subshell_destroy(&command->subshell);
    break;
  case command_loop:
    loop_destroy(&command->loop);
    break;
  case command_for_in:
    for_in_destroy(&command->for_in);
    break;
  case command_function:
    function_destroy(command->function);
    memory_dealloc(command->function);
    break;
  }
  command->type = command_none;
}

void command_init(struct command *command) {
  command->type = command_none;
  stack_init(&command->redirect);
}

void command_clear(struct command *command) {
  command_main_destroy(command);
  stack_clear(&command->redirect);
}

bool command_select(struct command *command, enum command_type type) {
  if (command->type == type) {
    switch (command->type) {
    case command_none:
      break;
    case command_keyword:
      command->keyword = keyword_none;
      break;
    case command_simple:
      simple_clear(&command->simple);
      break;
    case command_brace:
      brace_clear(&command->brace);
      break;
    case command_subshell:
      subshell_clear(&command->subshell);
      break;
    case command_loop:
      loop_clear(&command->loop);
      break;
    case command_for_in:
      for_in_clear(&command->for_in);
      break;
    case command_function:
      function_clear(command->function);
      break;
    }
  } else {
    command_destroy(command);
    command->type = type;
    switch (type) {
    case command_none:
      break;
    case command_keyword:
      command->keyword = keyword_none;
      break;
    case command_simple:
      simple_init(&command->simple);
      break;
    case command_brace:
      subshell_init(&command->subshell);
      break;
    case command_subshell:
      brace_init(&command->brace);
      break;
    case command_loop:
      loop_init(&command->loop);
      break;
    case command_for_in:
      for_in_init(&command->for_in);
      break;
    case command_function:
      command->function = memory_alloc(sizeof(struct function));
      if (command->function == NULL) {
        assert(errno == ENOMEM);
        throw(system, no_memory);
        return false;
      }
      function_init(command->function);
      break;
    }
  }
  return true;
}

void command_destroy(struct command *command) {
  command_main_destroy(command);
  struct redirect *redirect = command->redirect.data;
  size_t size = command->redirect.size / sizeof(struct redirect);
  for (size_t i = 0; i < size; i++) {
    redirect_destroy(&redirect[i]);
  }
  stack_destroy(&command->redirect);
}

bool command_expand(struct command command, struct command *target) {
  assert(!command.expanded);
  command_clear(target);
  switch (command.type) {
  case command_none:
  case command_keyword:
    break;
  case command_simple:
    if (!command_select(target, command_simple)) {
      return false;
    }
    return simple_expand(command.simple, &target->simple);
  case command_brace:
  case command_subshell:
  case command_loop:
  case command_for_in:
  case command_function:
    todo("Expansion of command other than simple");
    break;
  }
  target->expanded = true;
  return true;
}

bool command_execute(struct command command, struct context *context,
                     struct pipe *pipe) {
  assert(command.expanded);
  switch (command.type) {
  case command_none:
    assert(command.type != command_none);
    break;
  case command_keyword:
    assert(command.type != command_keyword);
    break;
  case command_simple:
    return simple_execute(command.simple, context, command.redirect,
                          pipe);
  case command_brace:
    break;
  case command_subshell:
    return block_execute(command.subshell.body, context, command.redirect, pipe);
  case command_loop:
    break;
  case command_for_in:
    break;
  case command_function: {
    struct string name;
    string_init(&name);
    if (!word_expand(command.function->name, &name)) {
      string_destroy(&name);
      return false;
    }
    hash_size_t index;
    if (!hash_find(context->function, name, &index)) {
      if (!hash_claim(&context->function, name, &index)) {
        string_destroy(&name);
        return false;
      }
    }
    struct command *body = hash_head(&context->function, index, 0);
    *body = command.function->body;
  } break;
  }
  return true;
}
