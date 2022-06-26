#ifndef NSH_MEMORY_H
#define NSH_MEMORY_H

#include <stddef.h>

void *memory_alloc(size_t size);

void *memory_realloc(void *space, size_t size);

void memory_dealloc(void *space);

#endif // NSH_MEMORY_H
