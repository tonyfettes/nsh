#include "nsh/string.h"

#include <stdio.h>

int main() {
  struct string s;
  string_init(&s);
  assert(s.size == 0);
  assert(s.capacity == sizeof(s.stack));
  for (size_t i = 0; i < sizeof(s.stack) - 1; i++) {
    assert(string_putc(&s, 'a'));
  }
  assert(s.capacity == sizeof(s.stack));
  assert(string_putc(&s, 'a'));
  assert(s.capacity > sizeof(s.stack));
  string_destroy(&s);

  string_init(&s);
  string_destroy(&s);
  return 0;
}
