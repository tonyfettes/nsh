#ifndef NSH_VARIABLE_H
#define NSH_VARIABLE_H

#include "nsh/string.h"
#include "nsh/hash.h"
#include "nsh/word.h"

// Even though variable should be all string, we could have underlying
// type for them.
struct variable {
  enum variable_type {
    variable_integer,
    variable_floating,
    variable_string,
    variable_array,
  } type;
  struct {
    bool read_only : 1;
    bool export : 1;
  } attribute;
  union {
    long integer;
    double floating;
    struct string string;
    // Stack of `struct variable`.
    struct stack array;
  };
};

void variable_init(struct variable *variable);

void variable_destroy(struct variable *variable);

void variable_select(struct variable *variable,
                     enum variable_type type);

bool variable_convert(struct variable *variable,
                      enum variable_type type);

struct assignment {
  struct string name;
  struct word value;
};

void assignment_init(struct assignment *assignment);

void assignment_destroy(struct assignment *assignment);

struct environment {
  struct string name;
  struct string value;
};

void environment_init(struct environment *environment);

void environment_clear(struct environment *environment);

void environment_destroy(struct environment *environment);

bool assignment_expand(struct assignment assignment,
                       struct environment *environment);

#endif // NSH_VARIABLE_H
