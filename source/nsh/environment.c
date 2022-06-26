#include "nsh/environment.h"

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
