#include "nsh/parameter.h"

#include <string.h>

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
