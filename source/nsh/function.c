#include "nsh/function.h"

void function_init(struct function *function) {
  word_init(&function->name);
  command_init(&function->body);
}

void function_clear(struct function *function) {
  word_clear(&function->name);
  command_clear(&function->body);
}

void function_destroy(struct function *function) {
  word_destroy(&function->name);
  command_destroy(&function->body);
}
