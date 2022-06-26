#include "nsh/stack.h"

#include <string.h>

enum { stack_capacity_min = 16 };

#undef stack_init
#undef stack_clear
#undef stack_destroy
#undef stack_tail
#undef stack_alloc
#undef stack_bump
#undef stack_push
#undef stack_back
#undef stack_fall
#undef stack_data

void stack_init(struct stack *stack) {
  stack->capacity = 0;
  stack->size = 0;
  stack->data = NULL;
}

void stack_clear(struct stack *stack) {
  stack->size = 0;
}

void stack_destroy(struct stack *stack) {
  // free(3p): If ptr is a null pointer, no action shall occur.
  memory_dealloc(stack->data);
  stack_init(stack);
}

void *stack_tail(struct stack *stack, stack_size_t size) {
  return (char *)stack->data + stack->size - size;
}

bool stack_alloc(struct stack *stack, stack_size_t size) {
  // Here `new_capacity` could overflow and loop forever, but having a 2
  // gigabytes stack is already a problem in the first place.
  stack_size_t new_capacity = stack->capacity;
  if (new_capacity == 0) {
    new_capacity = stack_capacity_min;
  }
  while (stack->size + size > new_capacity) {
    new_capacity = new_capacity + (new_capacity / 2);
  }
  if (new_capacity > stack->capacity) {
    void *allocated = memory_realloc(stack->data, new_capacity);
    if (allocated == NULL) {
      assert(errno == ENOMEM);
      throw(system, no_memory);
      return false;
    }
    stack->data = allocated;
    stack->capacity = new_capacity;
  }
  return true;
}

bool stack_bump(struct stack *stack, stack_size_t size) {
  try(stack_alloc(stack, size));
  stack->size += size;
  return true;
}

bool stack_push(struct stack *stack, void const *restrict source,
                stack_size_t size) {
  try(stack_alloc(stack, size));
  void *target = stack_tail(stack, 0);
  memcpy(target, source, size);
  stack->size += size;
  return true;
}

void stack_fall(struct stack *stack, stack_size_t size) {
  stack->size -= size;
}

void stack_back(struct stack *stack, void *restrict target,
                stack_size_t size) {
  memcpy(target, (char *)stack->data + stack->size - size, size);
}

void stack_pop(struct stack *stack, void *restrict target,
               stack_size_t size) {
  stack_back(stack, target, size);
  stack_fall(stack, size);
}
