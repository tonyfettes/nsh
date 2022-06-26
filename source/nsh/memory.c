#include "nsh/memory.h"

#include <stdlib.h>

void *memory_alloc(size_t size) {
  return malloc(size);
}

void *memory_realloc(void *space, size_t size) {
  return realloc(space, size);
}

void memory_dealloc(void *space) {
  free(space);
}
