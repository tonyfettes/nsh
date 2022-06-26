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
  struct word *argument = simple->argument.data;
  size_t size = simple->argument.size / sizeof(struct word);
  for (size_t i = 0; i < size; i++) {
    word_destroy(&argument[i]);
  }
  stack_destroy(&simple->argument);
  struct environment *env = simple->environment.data;
  size = simple->environment.size / sizeof(struct environment);
  for (size_t i = 0; i < size; i++) {
    environment_destroy(&env[i]);
  }
  stack_destroy(&simple->environment);
}
