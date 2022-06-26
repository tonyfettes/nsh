#ifndef NSH_ASSERT_H
#define NSH_ASSERT_H

#include <stdnoreturn.h>
#include "nsh/location.h"

extern char const *program;

#define location() (struct location) {                                 \
  .file = __FILE__,                                                    \
  .line = __LINE__,                                                    \
  .function = __func__,                                                \
}

void assert_at(struct location location,
               char const *restrict assertion);

#ifndef NDEBUG
#define assert(...)                                                    \
  if (__VA_ARGS__) {}                                                  \
  else assert_at(location(), #__VA_ARGS__)
#else
#define assert(...)
#endif

noreturn void panic_at(struct location location,
                       char const *restrict format, ...);

#define panic(...) panic_at(location(), "" __VA_ARGS__)

#define todo(feature)                                                  \
  panic_at(location(), feature" is not implemented.")

#define unreachable()                                                  \
  panic_at(location(), "Reached unreachable place.")

#endif // NSH_ASSERT_H
