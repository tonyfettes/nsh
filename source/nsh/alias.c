#include "nsh/alias.h"

bool alias_table_init(struct alias_table *table) {
  stack_init(&table->data);
  stack_init(&table->recycled);
  return hash_init(&table->hash, 0);
}

bool alias_table_insert(struct alias_table *table, struct string *name,
                        struct string *value) {
  // Get where alias value should go. No external status should be
  // affected before we do `hash_insert()`.
  struct string *target;
  bool use_recycled = table->recycled.size != 0;
  if (use_recycled) {
    stack_back(&table->recycled, &target, sizeof(struct string *));
  } else {
    try(stack_alloc(&table->data, sizeof(struct string)));
    target = stack_tail(&table->data, 0);
  }
  // Shallow copy `name` here to get a const reference to it so that we
  // could read from it later when insert failed.
  struct string name_cref = *name;
  void *temp = target;
  if (!hash_insert(&table->hash, name, &temp, true)) {
    return false;
  }
  if (temp != NULL) {
    // If we have overwritten an alias, collect its data pointer to
    // be recycled in future.
    struct string *overwritten = temp;
    if (use_recycled) {
      table->recycled.size -= sizeof(struct string *);
      assert(stack_push(&table->recycled, &overwritten,
                        sizeof(struct string *)));
    } else {
      if (!stack_push(&table->recycled, &target,
                     sizeof(struct string *))) {
        assert(hash_remove(&table->hash, name_cref, NULL));
        return false;
      }
    }
  }
  // If everything works correctly, move `value`.
  *target = *value;
  if (!use_recycled) {
    assert(stack_bump(&table->data, sizeof(struct string)));
  }
  string_init(value);
  return true;
}

bool alias_table_find(struct alias_table const *table,
                      struct string name, struct string **value) {
  void *temp;
  bool ok = hash_find(&table->hash, name, &temp);
  *value = temp;
  return ok;
}

void alias_table_destroy(struct alias_table *table) {
  hash_destroy(&table->hash);
  // Fill up holes, so that we could `string_destroy` them all.
  struct string **recycled = table->recycled.data;
  size_t size = table->recycled.size / sizeof(struct string *);
  for (size_t i = 0; i < size; i++) {
    string_init(recycled[i]);
  }
  stack_destroy(&table->recycled);

  struct string *data = table->data.data;
  size = table->data.size / sizeof(struct string);
  for (size_t i = 0; i < size; i++) {
    string_destroy(&data[i]);
  }
  stack_destroy(&table->data);
}
