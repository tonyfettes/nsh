#include "nsh/hash.h"

const int error_hash = 0;

static hash_size_t hash_calc(struct string name) {
  size_t hash = 0;
  char const *data = string_data(&name);
  for (size_t i = 0; data[i]; i++) {
    hash = hash * 131 + data[i];
  }
  return hash;
}

static hash_size_t find_key(struct hash const *hash,
                            hash_size_t hval, struct string key) {
  struct hash_entry *entry = hash->entry;
  hash_size_t pos = hval % hash->capacity;
  for (hash_size_t i = 1; entry[pos].type != hash_empty; i++) {
    if (string_equal(entry[pos].key, key)) {
      break;
    }
    pos = (pos + i * i) % hash->capacity;
  }
  return pos;
}

static hash_size_t find_slot(struct hash const *hash,
                             hash_size_t hval) {
  struct hash_entry *entry = hash->entry;
  size_t pos = hval % hash->capacity;
  for (size_t i = 1; entry[pos].type == hash_valid; i++) {
    pos = (pos + i * i) % hash->capacity;
  }
  return pos;
}

static void relocate(struct hash *hash, hash_size_t pos) {
  struct hash_entry *entry = hash->entry;
  struct hash_entry temp = entry[pos];
  entry[pos].type = hash_empty;
  hash_size_t h = hash_calc(entry[pos].key) % hash->capacity;
  for (size_t i = 1; entry[h].type == hash_rehashed; i++) {
    h = (h + i * i) % hash->capacity;
  }
  switch (entry[h].type) {
  case hash_valid:
    relocate(hash, h);
    // Fall through.
  case hash_empty:
  case hash_removed:
    entry[h] = temp;
    entry[h].type = hash_rehashed;
    break;
  case hash_rehashed:
    unreachable();
    break;
  }
}

static void rehash(struct hash *hash) {
  struct hash_entry *entry = hash->entry;
  for (size_t i = 0; i < hash->capacity; i++) {
    if (entry[i].type == hash_valid) {
      relocate(hash, i);
    }
  }
  for (size_t i = 0; i < hash->capacity; i++) {
    if (entry[i].type == hash_rehashed) {
      entry[i].type = hash_valid;
    }
  }
}

static bool grow(struct hash *hash) {
  size_t new_capacity = hash->capacity + (hash->capacity / 2);
  size_t realloc_size = new_capacity * sizeof(struct hash_entry);
  void *new_data = memory_realloc(hash->entry, realloc_size);
  if (new_data == NULL) {
    throw(system, no_memory);
    return false;
  }
  hash->entry = new_data;
  for (size_t i = hash->capacity; i < new_capacity; i++) {
    hash->entry[i].type = hash_empty;
  }
  hash->capacity = new_capacity;
  return true;
}

bool hash_init(struct hash *hash, hash_size_t capacity) {
  hash->size = 0;
  hash->capacity = capacity;
  if (hash->capacity == 0) {
    hash->capacity = 16;
  }
  size_t alloc_size = hash->capacity * sizeof(struct hash_entry);
  hash->entry = memory_alloc(alloc_size);
  if (hash->entry == NULL) {
    throw(system, no_memory);
    return false;
  }
  struct hash_entry *entry = hash->entry;
  for (size_t i = 0; i < hash->capacity; i++) {
    entry[i].type = hash_empty;
  }
  return true;
}

bool hash_insert(struct hash *hash, struct string *key, void **value,
                 bool overwrite) {
  struct hash_entry *entry = hash->entry;
  hash_size_t h = hash_calc(*key);
  hash_size_t pos = find_key(hash, h, *key);
  if (entry[pos].type == hash_valid) {
    if (!overwrite) {
      throw(hash, already_exist);
      return false;
    }
    void *overwritten = entry[pos].value;
    entry[pos].value = *value;
    *value = overwritten;
  } else {
    if ((hash->size + 1) > hash->capacity * 3 / 4) {
      try(grow(hash));
      entry = hash->entry;
      rehash(hash);
      pos = find_slot(hash, h);
    }
    entry[pos].type = hash_valid;
    entry[pos].key = *key;
    string_init(key);
    entry[pos].value = *value;
    *value = NULL;
    hash->size++;
  }
  return true;
}

bool hash_find(struct hash const *hash, struct string key,
               void **value) {
  struct hash_entry *entry = hash->entry;
  hash_size_t pos = find_key(hash, hash_calc(key), key);
  if (entry[pos].type != hash_valid) {
    throw(hash, not_exist);
    return false;
  }
  *value = entry[pos].value;
  return true;
}

bool hash_remove(struct hash *hash, struct string key, void **value) {
  struct hash_entry *entry = hash->entry;
  hash_size_t pos = find_key(hash, hash_calc(key), key);
  if (entry[pos].type != hash_valid) {
    throw(hash, not_exist);
    return false;
  }
  if (value) {
    *value = entry[pos].value;
  }
  entry[pos].type = hash_removed;
  hash->size--;
  return true;
}

void hash_destroy(struct hash *hash) {
  memory_dealloc(hash->entry);
  hash->size = 0;
  hash->capacity = 0;
  hash->entry = NULL;
}
