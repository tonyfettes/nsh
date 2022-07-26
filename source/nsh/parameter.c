#include "nsh/parameter.h"

#include <string.h>
#include <stdlib.h>

bool is_special_parameter(char ch) {
  return strchr("@*#?-$!", ch);
}

void parameter_init(struct parameter *parameter) {
  string_init(&parameter->name);
  word_init(&parameter->word);
}

void parameter_destroy(struct parameter *parameter) {
  string_destroy(&parameter->name);
  word_destroy(&parameter->word);
}

static bool expand_simple(struct parameter parameter,
                          struct string *target) {
  char const *value = getenv(string_data(&parameter.name));
  if (!value) {
    warn("envrionment variable `%s' is not set");
    return true;
  } else {
    return string_puts(target, value);
  }
}

static bool expand_default(struct parameter parameter,
                           struct string *target) {
  char const *value = getenv(string_data(&parameter.name));
  if (!value || (parameter.null_as_unset && value[0] == '\0')) {
    return word_expand(parameter.word, target);
  } else {
    return string_puts(target, value);
  }
}

static bool expand_assign(struct parameter parameter,
                          struct string *target) {
  char const *name = string_data(&parameter.name);
  char const *value = getenv(name);
  if (!value || (parameter.null_as_unset && value[0] == '\0')) {
    char const *new_value = string_tail(target, 0);
    if (!word_expand(parameter.word, target)) {
      return false;
    }
    if (setenv(name, new_value, true) == -1) {
      assert(errno != EINVAL);
      throw_system(errno);
      return false;
    }
    return true;
  } else {
    return string_puts(target, value);
  }
}

bool parameter_expand(struct parameter parameter,
                      struct string *target) {
  switch (parameter.type) {
  case parameter_simple:
    return expand_simple(parameter, target);
  case parameter_default:
    return expand_default(parameter, target);
  case parameter_assign:
    return expand_assign(parameter, target);
  case parameter_indicate:
    todo("Parameter expansion with indication of null");
  case parameter_alternate:
    todo("Parameter expansion with alternative");
  case parameter_length:
    todo("Parameter expansion as length");
  case parameter_suffix:
    todo("Parameter expansion subsitute suffix");
  case parameter_prefix:
    todo("Parameter expansion subsitute prefix");
  }
}
