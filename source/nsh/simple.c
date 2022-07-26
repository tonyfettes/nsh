#include "nsh/simple.h"

#include "nsh/word.h"
#include "nsh/environment.h"

void simple_init(struct simple *simple) {
  stack_init(&simple->argument);
  stack_init(&simple->environment);
}

void simple_clear(struct simple *simple) {
  stack_clear(&simple->argument);
  stack_clear(&simple->environment);
}

void simple_destroy(struct simple *simple) {
  for (struct word *argument = stack_head(&simple->argument, 0);
       argument != stack_tail(&simple->argument, 0); argument++) {
    word_destroy(argument);
  }
  stack_destroy(&simple->argument);
  for (struct environment *env = stack_head(&simple->environment, 0);
       env != stack_tail(&simple->environment, 0); env++) {
    environment_destroy(env);
  }
  stack_destroy(&simple->environment);
}

bool simple_display(struct simple simple, struct string *target) {
  for (struct environment *env = stack_head(&simple.environment, 0);
       env != stack_tail(&simple.environment, 0); env++) {
    if (target->size) {
      try(string_putc(target, ' '));
    }
    try(environment_display(env, target));
  }
  for (struct word *arg = stack_head(&simple.argument, 0);
       arg != stack_head(&simple.argument, 0); arg++) {
    if (!word_expand(*arg, target)) {
      return false;
    }
  }
  return true;
}
