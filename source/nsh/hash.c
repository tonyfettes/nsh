#include "nsh/hash.h"

char const *error_hash(int code) {
  static char const *const description[] = {
    [error_hash_not_exist] = "No such string in hash table",
    [error_hash_already_exist] = "String exists",
  };
  return description[code];
}

static bool should_grow(struct hash hash) {
  return (hash.size + 1) > (hash.capacity * 3 / 4);
}

static hash_size_t calculate(char const *key) {
  size_t hash = 0;
  for (size_t i = 0; key[i]; i++) {
    hash = hash * 131 + key[i];
  }
  return hash;
}

static hash_size_t find_key(struct hash hash, hash_size_t hval,
                            struct string key) {
  struct hash_entry *entry = hash.entry;
  hash_size_t pos = hval % hash.capacity;
  for (hash_size_t i = 1; entry[pos].type != hash_empty; i++) {
    if (strcmp(entry[pos].key.data, string_data(&key)) == 0) {
      break;
    }
    pos = (pos + i * i) % hash.capacity;
  }
  return pos;
}

static hash_size_t find_slot(struct hash hash, hash_size_t hval) {
  struct hash_entry *entry = hash.entry;
  size_t pos = hval % hash.capacity;
  for (hash_size_t i = 1; entry[pos].type == hash_valid; i++) {
    pos = (pos + i * i) % hash.capacity;
  }
  return pos;
}

static void relocate(struct hash *hash, hash_size_t pos) {
  struct hash_entry *entry = hash->entry;
  struct hash_entry temp = entry[pos];
  entry[pos].type = hash_empty;
  hash_size_t h = calculate(entry[pos].key.data) % hash->capacity;
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
  if (new_capacity == 0) {
    new_capacity = 4;
  }
  size_t realloc_size = new_capacity * sizeof(struct hash_entry);
  void *new_data = memory_realloc(hash->entry, realloc_size);
  if (new_data == NULL) {
    throw_system(errno);
    return false;
  }
  hash->entry = new_data;
  for (size_t i = hash->capacity; i < new_capacity; i++) {
    hash->entry[i].type = hash_empty;
    stack_init(&hash->entry[i].key);
    hash->entry[i].data_offset = 0;
  }
  hash->capacity = new_capacity;
  return true;
}

void hash_init(struct hash *hash) {
  hash->size = 0;
  hash->capacity = 0;
  hash->entry = null;
}

bool hash_assign(struct hash *hash, struct hash source) {
  if (hash->entry) {
    hash_destroy(hash);
  }
  size_t alloc_size = source.capacity * sizeof(struct hash_entry);
  hash->entry = memory_alloc(alloc_size);
  if (hash->entry == NULL) {
    throw_system(errno);
    return false;
  }
  for (hash_size_t i = 0; i < source.capacity; i++) {
    hash->entry[i].type = source.entry[i].type;
    stack_init(&hash->entry[i].key);
    if (!stack_assign(&hash->entry[i].key, source.entry[i].key)) {
      return false;
    }
    hash->entry[i].data_offset = source.entry[i].data_offset;
  }
  return true;
}

bool hash_find(struct hash hash, struct string key,
               hash_size_t *index) {
  struct hash_entry *entry = hash.entry;
  hash_size_t hval = calculate(string_data(&key));
  hash_size_t pos = find_key(hash, hval, key);
  if (entry[pos].type != hash_valid) {
    throw(hash, not_exist);
    return false;
  }
  *index = pos;
  return true;
}

static stack_size_t ceil_to_max_align(stack_size_t size) {
  enum { max_align = alignof(max_align_t) };
  return (size + max_align - 1) / max_align * max_align;
}

bool hash_claim(struct hash *hash, struct string key,
                hash_size_t *index) {
  struct hash_entry *entry = hash->entry;
  hash_size_t hval = calculate(string_data(&key));
  hash_size_t pos = find_key(*hash, hval, key);
  if (entry[pos].type == hash_valid) {
    *index = pos;
    return true;
  }
  if (should_grow(*hash)) {
    try(grow(hash));
    rehash(hash);
    entry = hash->entry;
    pos = find_slot(*hash, hval);
  }
  entry[pos].data_offset = ceil_to_max_align(key.size + 1);
  stack_resize(&entry[pos].key, entry[pos].data_offset);
  memory_copy(stack_head(&entry[pos].key, 0), string_data(&key),
              key.size);
  memory_fill(stack_head(&entry[pos].key, key.size),
              entry[pos].data_offset - key.size, 0);
  *index = pos;
  return true;
}

void hash_insert(struct hash *hash, hash_size_t index) {
  assert(index < hash->capacity);
  if (hash->entry[index].type != hash_valid) {
    hash->size++;
    hash->entry[index].type = hash_valid;
  }
}

void hash_remove(struct hash *hash, hash_size_t index) {
  struct hash_entry *entry = hash->entry;
  assert(index < hash->capacity);
  if (entry[index].type == hash_valid) {
    hash->size--;
    entry[index].type = hash_removed;
  }
}

void hash_destroy(struct hash *hash) {
  memory_dealloc(hash->entry);
  hash_init(hash);
}
