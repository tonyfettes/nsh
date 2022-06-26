#include "nsh/stack.h"

#include <stdlib.h>
#include <stdio.h>

bool fail_alloc = false;

void *memory_alloc(size_t size) {
  if (fail_alloc) {
    errno = ENOMEM;
    return NULL;
  } else {
    return malloc(size);
  }
}

void *memory_realloc(void *space, size_t size) {
  if (fail_alloc) {
    errno = ENOMEM;
    return NULL;
  } else {
    return realloc(space, size);
  }
}

void memory_dealloc(void *space) {
  free(space);
}

int main() {
  struct stack s;
  stack_init(&s);
  assert(s.size == 0);
  assert(s.capacity == 0);
  assert(s.data == NULL);

  int element = 0xDEADBEEF;
  assert(stack_alloc(&s, sizeof(element)));
  assert(stack_bump(&s, sizeof(element)));
  fprintf(stderr, "s.size: %d\n", s.size);
  assert(s.size == sizeof(element));
  assert(s.capacity >= sizeof(element));
  assert(s.data != NULL);

  fail_alloc = true;
  struct stack old;
  while (stack_push(&s, &element, sizeof(element))) {
    old = s;
  }
  assert(s.size == old.size);
  assert(s.capacity == old.capacity);
  assert(s.data == old.data);
  for (size_t i = 0; i < s.capacity; i++) {
    i[(char *)s.data] = 0;
  }

  stack_destroy(&s);
  assert(s.size == 0);
  assert(s.capacity == 0);
  assert(s.data == NULL);
  return 0;
}
