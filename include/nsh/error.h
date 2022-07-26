#ifndef NSH_ERROR_H
#define NSH_ERROR_H

#include <stdbool.h>
#include <errno.h>

#define try(...) if (__VA_ARGS__) {} else return false

extern struct error {
  char const *(*category)(int code);
  int code;
} error;

extern const int error_system;

enum error_system {
  error_system_again = EAGAIN,
  error_system_no_memory = ENOMEM,
  error_system_not_supported = ENOSYS,
};

#define catch(category_in, code_in)                                    \
  (error.category == &error_##category_in &&                           \
   error.code == error_##category_in##_##code_in)   

#define throw(category_in, code_in)                                    \
  (void) (error.category = &error_##category_in,                       \
          error.code = error_##category_in##_##code_in)

void throw_system(int error_code);

#endif // NSH_ERROR_H
