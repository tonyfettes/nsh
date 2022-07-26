#ifndef NSH_ASSERT_H
#define NSH_ASSERT_H

#include <stdnoreturn.h>
#include "nsh/location.h"

void assert_at(struct location location,
               char const *restrict assertion);

#ifndef NDEBUG
#define assert(...)                                                    \
  if (__VA_ARGS__) {} else assert_at(locate(), #__VA_ARGS__)
#else
#define assert(...)
#endif

noreturn void panic_at(struct location location,
                       char const *restrict format, ...);

#define panic(...) panic_at(locate(), "" __VA_ARGS__)

#define todo(feature)                                                  \
  panic_at(locate(), feature" is not implemented.")

#define unreachable()                                                  \
  panic_at(locate(), "Reached unreachable place.")

#endif // NSH_ASSERT_H
