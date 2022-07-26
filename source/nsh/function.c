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

void function_table_init(struct function_table *table) {
  hash_init(&table->hash);
}

void function_table_destroy(struct function_table *table) {
  hash_destroy(&table->hash);
}

void function_table_insert(struct function_table *table,
                           struct function function) {
  hash_size_t index;
  if (!hash_claim(&table->hash, function.name, &index)) {
    return false;
  }
  if (!hash_alloc(&table->hash, index, sizeof(struct command))) {
    return false;
  }
}
