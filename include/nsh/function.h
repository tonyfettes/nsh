#ifndef NSH_FUNCTION_H
#define NSH_FUNCTION_H

#include "nsh/word.h"
#include "nsh/command.h"
#include "nsh/string.h"
#include "nsh/hash.h"
#include "nsh/stack.h"

struct function {
  struct word name;
  struct command body;
};

void function_init(struct function *function);

void function_clear(struct function *function);

bool function_execute(struct function const *function,
                      struct simple const *simple);

void function_destroy(struct function *function);

struct function_table {
  struct hash hash;
};

void function_table_init(struct function_table *table);

void function_table_destroy(struct function_table *table);

void function_table_insert(struct function_table *table,
                           struct function function);

void function_table_find(struct function_table *function_table,
                         struct string name, struct command *body);

void function_table_remove(struct function_table *function_table,
                           struct string name);

#endif // NSH_FUNCTION_H
