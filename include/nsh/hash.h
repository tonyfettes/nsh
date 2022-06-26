#ifndef NSH_HASH_H
#define NSH_HASH_H

#include "nsh/string.h"

typedef unsigned int hash_size_t;

struct hash_entry {
  enum {
    hash_empty,
    hash_valid,
    hash_removed,
    hash_rehashed,
  } type;
  struct string key;
  void *value;
};

struct hash {
  hash_size_t capacity;
  hash_size_t size;
  struct hash_entry *entry;
};

enum error_hash {
  error_hash_not_exist,
  error_hash_already_exist,
};

extern const int error_hash;

bool hash_init(struct hash *hash, hash_size_t capacity);

bool hash_insert(struct hash *hash, struct string *key, void **value,
                 bool overwrite);

bool hash_find(struct hash const *hash, struct string key,
               void **value);

bool hash_remove(struct hash *hash, struct string key, void **value);

void hash_destroy(struct hash *hash);

#endif // NSH_HASH_H
