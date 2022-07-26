#include "nsh/execute.h"
#include "nsh/context.h"

static struct string const ifs_key = {
  .capacity = 4,
  .size = 3,
  .stack = "IFS",
};

bool execute_for_in(struct stack *job, struct for_in for_in) {
  struct string ifs;
  string_init(&ifs);
  hash_size_t index;
  if (!hash_find(context.variable, ifs_key, &index)) {
    if (!string_puts(&ifs, " \t\n\r")) {
      return false;
    }
  }
  struct stack value;
  stack_init(&value);
  {
    struct word *head = stack_head(&for_in.word_list, 0);
    struct word *tail = stack_tail(&for_in.word_list, 0);
    for (struct word *word = head; word != tail; word++) {
      if (!word_split(*word, &value, ifs)) {
        stack_destroy(&value);
        return false;
      }
    }
  }
  struct variable *variable;
  if (hash_find(context.variable, for_in.name, &index)) {
    variable = hash_head(&context.variable, index, 0);
    if (!variable_convert(variable, variable_string)) {
      stack_destroy(&value);
      return false;
    }
    struct string *head = stack_head(&value, 0);
    struct string *tail = stack_tail(&value, 0);
    for (struct string *string = head; string != tail; string++) {
      string_swap(&variable->string, string);
      if (!execute_block(job, for_in.body)) {
        stack_destroy(&value);
        return false;
      }
    }
  } else {
    if (!hash_claim(&context.variable, for_in.name, &index)) {
      stack_destroy(&value);
      return false;
    }
    if (!hash_reserve(&context.variable, index,
                      sizeof(struct variable))) {
      stack_destroy(&value);
      return false;
    }
    variable = hash_head(&context.variable, index, 0);
  }
  return true;
}
