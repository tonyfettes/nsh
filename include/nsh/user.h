#ifndef NSH_USER_H
#define NSH_USER_H

#include "nsh/string.h"


enum error_user {
  error_user_not_found,
};

char const *error_user(int code);

bool user_home(struct string name, struct string *home);

#endif // NSH_USER_H
