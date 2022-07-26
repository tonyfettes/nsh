#ifndef NSH_ENVIRONMENT_H
#define NSH_ENVIRONMENT_H

#include "nsh/string.h"
#include "nsh/word.h"

extern char **environ;

struct environment {
  struct string name;
  struct string value;
};

void environment_init(struct environment *environment);

void environment_clear(struct environment *environment);

void environment_destroy(struct environment *environment);

bool environment_display(struct environment const *environment,
                         struct string *target);

#endif /* NSH_ENVIRONMENT_H */
