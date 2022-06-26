#include "nsh/hash.h"

#include <stdio.h>

int main() {
  struct hash h;
  assert(hash_init(&h, 2));
  assert(h.size == 0);
  assert(h.capacity == 2);
  assert(h.entry != NULL);

  struct string s;
  string_init(&s);
  assert(string_puts(&s, "abc"));

  int data[10];
  for (size_t i = 0; i < 10; i++) {
    data[i] = i;
  }
  void *value = &data[0];
  assert(hash_insert(&h, &s, &value, true));
  assert(h.size == 1);
  assert(s.size == 0);
  assert(value == NULL);

  void *found = NULL;
  string_init(&s);
  assert(string_puts(&s, "abc"));
  assert(hash_find(&h, s, &found));
  assert(found == &data[0]);

  string_clear(&s);
  string_puts(&s, "def");
  assert(hash_find(&h, s, &found) == false);
  assert(catch(hash, not_exist));

  value = &data[1];
  assert(hash_insert(&h, &s, &value, true));
  assert(h.size == 2);

  string_init(&s);
  string_puts(&s, "abc");
  assert(hash_find(&h, s, &found));
  assert(hash_remove(&h, s, &found));

  string_clear(&s);
  string_puts(&s, "def");
  assert(hash_find(&h, s, &found));
  assert(hash_remove(&h, s, &found));

  string_clear(&s);
  string_puts(&s, "ghi");
  assert(hash_remove(&h, s, &found) == false);
  assert(catch(hash, not_exist));

  string_destroy(&s);
  hash_destroy(&h);
  return 0;
}
