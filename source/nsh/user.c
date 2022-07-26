#include "nsh/user.h"

#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>

char const *error_user(int code) {
  static char const *const description[] = {
    [error_user_not_found] = "No such user",
  };
  assert(code < sizeof(description) / sizeof(description[0]));
  return description[code];
}

bool user_home(struct string name, struct string *home_directory) {
  struct passwd *pw;
  if (name.size == 0) {
    char const *home_env = getenv("HOME");
    if (home_env) {
      return string_puts(home_directory, home_env);
    }
    pw = getpwuid(getuid());
  } else {
    pw = getpwnam(string_data(&name));
  }
  errno = 0;
  if (pw == null) {
    if (errno == 0) {
      throw(user, not_found);
    } else {
      throw_system(errno);
    }
    return false;
  }
  return string_puts(home_directory, pw->pw_dir);
}
