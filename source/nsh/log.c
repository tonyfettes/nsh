#include "nsh/log.h"

#include <stdio.h>
#include <stdarg.h>

void log_level_at(struct location location, enum log_level level,
                  char const *restrict format, ...) {
  static char const *const level_display[] = {
    [log_level_trace] = "Trace",
    [log_level_fixme] = "Fixme",
    [log_level_debug] = "Debug",
    [log_level_info] = "Information",
    [log_level_warn] = "Warning",
    [log_level_error] = "Error",
    [log_level_fatal] = "Fatal",
  };
  fprintf(stderr, "%s%s%s:%u: %s%s%s: ", program,
          program[0] ? ": " : "", location.file, location.line,
          location.function ? location.function : "",
          location.function ? ": " : "", level_display[level]);
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputc('\n', stderr);
}
