#ifndef NSH_ENVIRONMENT_H
#define NSH_ENVIRONMENT_H

#include "nsh/string.h"
#include "nsh/word.h"

extern char **environ;

struct environment {
  struct string name;
  struct word value;
};

void environment_init(struct environment *environment);

void environment_clear(struct environment *environment);

void environment_destroy(struct environment *environment);

#endif /* NSH_ENVIRONMENT_H */
