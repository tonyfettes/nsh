#include "nsh/context.h"

struct context context;

void context_init(struct context *context) {
  alias_table_init(&context->alias_table);
  // function_table_init(&context->function_table);
}

void context_destroy(struct context *context) {
  alias_table_destroy(&context->alias_table);
}
