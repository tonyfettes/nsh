#include "nsh/variable.h"

#include <stdlib.h>

extern char **environ;

void variable_init(struct variable *variable) {
  variable->type = variable_string;
  string_init(&variable->string);
}

void variable_destroy(struct variable *variable) {
  if (variable->type == variable_string) {
    string_destroy(&variable->string);
  } else {
    variable->type = variable_string;
  }
}

void variable_select(struct variable *variable,
                     enum variable_type type) {
  variable_destroy(variable);
  variable->type = type;
  if (type == variable_string) {
    string_init(&variable->string);
  }
}

bool variable_convert(struct variable *variable,
                      enum variable_type type) {
  switch (variable->type) {
  case variable_integer: {
    long integer = variable->integer;
    switch (type) {
    case variable_integer:
      return true;
    case variable_floating:
      variable->floating = (double)integer;
      return true;
    case variable_string:
      string_init(&variable->string);
      return string_printf(&variable->string, "%ld", integer);
    }
  } break;
  case variable_floating: {
    double floating = variable->floating;
    switch (type) {
    case variable_integer:
      variable->integer = (long)floating;
      return true;
    case variable_floating:
      return true;
    case variable_string:
      string_init(&variable->string);
      return string_printf(&variable->string, "%lf", floating);
    }
  } break;
  case variable_string: {
    struct string string = variable->string;
    char *end;
    errno = 0;
    switch (type) {
    case variable_integer:
      variable->integer = strtol(string_data(&string), &end, 0);
      break;
    case variable_floating:
      variable->floating = strtod(string_data(&string), &end);
      break;
    case variable_string:
      return true;
    }
    if (end != string_tail(&string, 0)) {
      todo("Handling invalid string");
      return false;
    }
    if (errno == ERANGE) {
      todo("Handling out of range conversion");
      return false;
    }
    return true;
  } break;
  }
}

void variable_import(struct hash *table) {
  for (char **env = environ; *env; env++) {
    size_t len = strchr(*env, '=');
    struct string name;
    string_init(&name);
    if (!string_putn(&name, *env, len)) {
      string_destroy(&name);
      return false;
    }
    hash_size_t index;
    if (!hash_claim(table, name, &index)) {
      return false;
    }
  }
}
