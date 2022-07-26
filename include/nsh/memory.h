# ifndef NSH_MEMORY_H
#define NSH_MEMORY_H

#include <stddef.h>

static void *const null = NULL;

void *memory_alloc(size_t size);

void *memory_realloc(void *space, size_t size);

void memory_dealloc(void *space);

void memory_swap(void *restrict target, void *restrict source,
                 size_t size);

void memory_copy(void *restrict target, void const *restrict source,
                 size_t size);

void memory_fill(void *restrict target, size_t size, char ch);

#endif // NSH_MEMORY_H
