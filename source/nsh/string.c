#include "nsh/string.h"

enum string_type {
  string_heap,
  string_stack,
};

static inline enum string_type string_type(struct string string) {
  if (string.capacity == string_stack_capacity) {
    return string_stack;
  } else {
    return string_heap;
  }
}

void string_init(struct string *string) {
  string->size = 0;
  string->capacity = string_stack_capacity;
  memset(string->stack, 0, sizeof(string->stack));
}

void string_clear(struct string *string) {
  string->size = 0;
  string_data(string)[string->size] = '\0';
}

void string_destroy(struct string *string) {
  if (string_type(*string) == string_heap) {
    memory_dealloc(string->heap);
  }
  string_init(string);
}

char *string_data(struct string *string) {
  if (string->capacity == string_stack_capacity) {
    return string->stack;
  } else {
    return string->heap;
  }
}

bool string_alloc(struct string *string, string_size_t size) {
  size_t new_size = string->size + size;
  size_t new_capacity = string->capacity;
  while (new_size > new_capacity) {
    new_capacity = new_capacity + (new_capacity / 2);
  }
  if (new_capacity > string->capacity) {
    void *allocated;
    if (string->capacity == string_stack_capacity) {
      allocated = memory_alloc(new_capacity);
      if (allocated == NULL) {
        goto alloc_failure;
      }
      memcpy(allocated, string->stack, string->size);
    } else {
      allocated = memory_realloc(string->heap, new_capacity);
      if (allocated == NULL) {
        goto alloc_failure;
      }
    }
    string->heap = allocated;
    string->capacity = new_capacity;
  }
  return true;

alloc_failure:
  assert(errno == ENOMEM);
  throw(system, no_memory);
  return false;
}

bool string_puts(struct string *string, char const *restrict s) {
  size_t size = strlen(s);
  try(string_alloc(string, size + 1));
  char *data = string_data(string);
  memcpy(data, s, size + 1);
  string->size += size;
  return true;
}

bool string_putc(struct string *string, char ch) {
  try(string_alloc(string, 2));
  char *data = string_data(string);
  data[string->size] = ch;
  string->size++;
  data[string->size] = '\0';
  return true;
}

bool string_append(struct string *string, struct string source) {
  try(string_alloc(string, source.size + 1));
  char *t_data = string_data(string);
  char *s_data = string_data(&source);
  char *target = t_data + string->size;
  memcpy(target, s_data, source.size + 1);
  string->size += source.size;
  return true;
}

bool string_assign(struct string *string, struct string source) {
  string_clear(string);
  return string_append(string, source);
}

void string_swap(struct string *string, struct string *other) {
  struct string temp = *other;
  *other = *string;
  *string = temp;
}

int string_compare(struct string left, struct string right) {
  char const *l_data = string_data(&left);
  char const *r_data = string_data(&right);
  for (size_t i = 0; i < left.size && i < right.size; i++) {
    int delta = l_data[i] - r_data[i];
    if (delta) {
      return delta;
    }
  }
  return left.size - right.size;
}

bool string_equal(struct string left, struct string right) {
  if (left.size != right.size) {
    return false;
  }
  char const *l_data = string_data(&left);
  char const *r_data = string_data(&right);
  return strncmp(l_data, r_data, left.size) == 0;
}
