#include "nsh/environment.h"
#include "nsh/format.h"

void environment_init(struct environment *environment) {
  string_init(&environment->name);
  word_init(&environment->value);
}

void environment_clear(struct environment *environment) {
  string_clear(&environment->name);
  word_clear(&environment->value);
}

void environment_destroy(struct environment *environment) {
  string_destroy(&environment->name);
  word_destroy(&environment->value);
}

bool environment_print(struct environment environment,
                       struct string *target) {
  string_printf(target, fs(string)"=%s", fa(environment.name), "");
  // try(string_concat(target, environment->name));
  // try(string_putc(target, '='));
  // assert(environment->value.type == word_plain);
  // try(string_concat(target, environment->value.plain));
  return false;
}
