#ifndef NSH_STRING_H
#define NSH_STRING_H

#include "nsh/common.h"
#include "nsh/stack.h"

#include <string.h>

enum { string_stack_capacity = 8 };

typedef unsigned int string_size_t;

struct string {
  string_size_t size;
  string_size_t capacity;
  union {
    char *heap;
    char stack[string_stack_capacity];
  };
};

void string_init(struct string *string);

void string_clear(struct string *string);

void string_destroy(struct string *string);

char *string_data(struct string *string);

// Allocate `size` bytes of space after the end of the string.
bool string_alloc(struct string *string, string_size_t size);

bool string_puts(struct string *string, char const *restrict s);

bool string_putc(struct string *string, char ch);

bool string_append(struct string *string, struct string source);

bool string_assign(struct string *string, struct string source);

void string_swap(struct string *string, struct string *other);

int string_compare(struct string left, struct string right);

bool string_equal(struct string left, struct string right);

bool string_split(struct string string, struct stack *splited,
                  struct string delimiter);

#endif // NSH_STRING_H
