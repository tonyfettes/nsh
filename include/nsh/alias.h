#ifndef NSH_ALIAS_H
#define NSH_ALIAS_H

#include "nsh/hash.h"
#include "nsh/stack.h"
#include "nsh/string.h"

struct alias_table {
  struct hash hash;
};

void alias_table_init(struct alias_table *table);

bool alias_table_insert(struct alias_table *table, struct string name,
                        struct string value);

bool alias_table_find(struct alias_table table, struct string name,
                      struct string *value);

void alias_table_destroy(struct alias_table *table);

#endif // NSH_ALIAS_H
