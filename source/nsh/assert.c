#include "nsh/assert.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

char const *program = "";

void assert_at(struct location location,
               char const *restrict assertion) {
  fprintf(stderr,
          "%s%s" // Program name.
          "%s:%u: " // File name and line number.
          "%s" // Function name.
          "%s" // ": " that follows function name.
          "Assertion `%s' failed.\n",
          program, program[0] ? ": " : "",
          location.file, location.line,
          location.function ? location.function : "",
          location.function ? ": " : "",
          assertion);
  exit(EXIT_FAILURE);
}

noreturn void panic_at(struct location location,
                       char const *restrict format, ...) {
  fprintf(stderr,
          "%s%s" // Program name.
          "%s:%u: " // File name and line number.
          "%s" // Function name.
          "%s" // ": " that follows function name.
          "Panicked"
          "%s", // ": " that follows "Panicked".
          program, program[0] ? ": " : "",
          location.file, location.line,
          location.function ? location.function : "",
          location.function ? ": " : "",
          format[0] ? ": " : ".");
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputc('\n', stderr);
  exit(EXIT_FAILURE);
}
