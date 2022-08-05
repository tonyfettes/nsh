#ifndef NSH_HASH_H
#define NSH_HASH_H

#include "nsh/string.h"
#include <stdalign.h>

typedef unsigned int hash_size_t;

struct hash_entry {
  enum hash_entry_type {
    hash_empty,
    hash_valid,
    hash_removed,
    // Only used in rehashing.
    hash_rehashed,
  } type;
  stack_size_t data_offset;
  struct stack key;
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

char const *error_hash(int code);

void hash_init(struct hash *hash);

bool hash_assign(struct hash *hash, struct hash source);

bool hash_find(struct hash hash, struct string key, hash_size_t *index);

bool hash_claim(struct hash *hash, struct string key,
                hash_size_t *index);

void *hash_head(struct hash *hash, hash_size_t index,
                stack_size_t size);

void *hash_tail(struct hash *hash, hash_size_t index,
                stack_size_t size);

bool hash_alloc(struct hash *hash, hash_size_t index,
                stack_size_t size);

bool hash_reserve(struct hash *hash, hash_size_t index,
                  stack_size_t size);

bool hash_bump(struct hash *hash, hash_size_t index, stack_size_t size);

void hash_insert(struct hash *hash, hash_size_t index);

void hash_remove(struct hash *hash, hash_size_t index);

void hash_destroy(struct hash *hash);

#endif // NSH_HASH_H
