#include "nsh/alias.h"
#include <stdio.h>

int main() {
  struct alias_table table;
  alias_table_init(&table);

  struct string name;
  string_init(&name);
  assert(string_puts(&name, "ll"));

  struct string value;
  string_init(&value);
  assert(string_puts(&value, "ls -lha"));

  struct string query_name = name;
  struct string query_value = value;

  assert(alias_table_insert(&table, &name, &value));
  struct string *found;
  assert(alias_table_find(&table, query_name, &found));
  assert(string_equal(*found, query_value));

  string_destroy(&value);
  string_destroy(&name);
  alias_table_destroy(&table);
  return 0;
}
