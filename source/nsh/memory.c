#include "nsh/memory.h"

#include <stdlib.h>
#include <stdalign.h>
#include <string.h>

void *memory_alloc(size_t size) {
  return malloc(size);
}

void *memory_realloc(void *space, size_t size) {
  return realloc(space, size);
}

void memory_dealloc(void *space) {
  free(space);
}

void memory_swap(void *restrict target, void *restrict source,
                 size_t size) {
  char *tptr = target;
  char *sptr = source;
  while (size--) {
    char temp = *tptr;
    *tptr = *sptr;
    *sptr = temp;
  }
}

void memory_copy(void *restrict target, void const *restrict source,
                 size_t size) {
  memcpy(target, source, size);
}

void memory_fill(void *restrict target, size_t size, char ch) {
  memset(target, ch, size);
}
