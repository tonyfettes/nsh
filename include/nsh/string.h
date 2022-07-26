#ifndef NSH_STRING_H
#define NSH_STRING_H

#include "nsh/common.h"
#include "nsh/stack.h"

#include <string.h>
#include <stdarg.h>

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

char *string_head(struct string *string, size_t size);

char *string_tail(struct string *string, size_t size);

char const *string_head_const(struct string const *string, size_t size);

char const *string_tail_const(struct string const *string, size_t size);

// Allocate `size` bytes of space after the end of the string.
bool string_alloc(struct string *string, string_size_t size);

bool string_puts(struct string *string, char const *restrict s);

bool string_putn(struct string *string, char const *restrict s,
                 size_t n);

bool string_putc(struct string *string, char ch);

bool string_concat(struct string *string, struct string source);

bool string_append(struct string *string, struct string source,
                   string_size_t from, string_size_t to);

bool string_intc(struct string *string, string_size_t position,
                 char ch);

bool string_ints(struct string *string, string_size_t position,
                 char const *restrict source);

bool string_intn(struct string *string, string_size_t position,
                 char const *restrict source, string_size_t size);

bool string_insert(struct string *string, string_size_t position,
                   struct string source);

bool string_remove(struct string *string, string_size_t position,
                   string_size_t size);

bool string_assign(struct string *string, struct string source);

void string_swap(struct string *string, struct string *other);

int string_compare(struct string left, struct string right);

bool string_equal(struct string left, struct string right);

bool string_find(struct string string, char ch, string_size_t *index);

bool string_count(struct string string, char ch, string_size_t *count);

bool string_count_in(struct string string, char ch, string_size_t from,
                     string_size_t to, string_size_t *count);

bool string_split(struct string string, struct stack *split,
                  struct string delimiter);

bool string_print(struct string string, struct string *target);

bool string_printf(struct string *string, char const *restrict format,
                   ...);

bool string_vprintf(struct string *string, char const *restrict format,
                    va_list args);

void string_place(struct string *string, char *restrict buffer,
                  string_size_t size, string_size_t capacity);

#define string_format_specifier "%s"
#define string_format_argument(var) string_data(&(var))

#endif // NSH_STRING_H
