#ifndef NSH_STACK_H
#define NSH_STACK_H

#include <stdbool.h>

typedef unsigned int stack_size_t;

// Generic stack. No alignment guarantees.
struct stack {
  stack_size_t capacity;
  stack_size_t size;
  void *data;
};

// Initialize `stack`, **won't allocate memory**.
void stack_init(struct stack *stack);

// Set `stack->size` to zero.
void stack_clear(struct stack *stack);

// Free the space `stack` holds, if any.
void stack_destroy(struct stack *stack);

void *stack_head(struct stack *stack, stack_size_t size);

void *stack_tail(struct stack *stack, stack_size_t size);

bool stack_reserve(struct stack *stack, stack_size_t size);

// Ensure there are at least `size` bytes of space available after
// the tail of `stack`.
bool stack_alloc(struct stack *stack, stack_size_t size);

bool stack_resize(struct stack *stack, stack_size_t size);

// Increase `stack->size` by `size`, realloc if necessary.
bool stack_bump(struct stack *stack, stack_size_t size);

// Copy `[source, source + size)` to the space right after the tail of
// the `stack`, and increase `stack->size`.
bool stack_push(struct stack *stack, void const *restrict source,
                stack_size_t size);

// Decearse `stack->size` by `size`.
void stack_fall(struct stack *stack, stack_size_t size);

// Copy the last `size` bytes into `target`.
void stack_back(struct stack *stack, void *restrict target,
                stack_size_t size);

void stack_pop(struct stack *stack, void *restrict target,
               stack_size_t size);

void stack_insert(struct stack *stack, stack_size_t position,
                  void const *restrict source, stack_size_t size);

void stack_remove(struct stack *stack, stack_size_t position,
                  void *restrict source, stack_size_t size);

#endif // NSH_STACK_H
