#include "nsh/assert.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

char const *program = "";

void assert_at(struct location location,
               char const *restrict assertion) {
  fprintf(stderr, "%s%s%s:%u: %s%sAssertion `%s' failed.\n", program,
          program[0] ? ": " : "", location.file, location.line,
          location.function ? location.function : "",
          location.function ? ": " : "", assertion);
  exit(EXIT_FAILURE);
}

noreturn void panic_at(struct location location,
                       char const *restrict format, ...) {
  fprintf(stderr, "%s%s%s:%u: %s%sPanicked%s", program,
          program[0] ? ": " : "", location.function, location.line,
          location.function ? location.function : "",
          location.function ? ": " : "", format[0] ? ": " : ".");
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  fputc('\n', stderr);
  va_end(args);
  exit(EXIT_FAILURE);
}
