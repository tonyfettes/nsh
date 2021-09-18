#include "log.h"

#include <stdio.h>
#include <stdlib.h>

noreturn
void panic(const char *restrict format, ...) {
  va_list args;
  va_start(args, format);
  fprintf(stderr, "fatal: ");
  vfprintf(stderr, format, args);
  fprintf(stderr, "\nexiting\n");
  exit(1);
}

void error(const char *restrict format, ...) {
  va_list args;
  va_start(args, format);
  fprintf(stderr, "error: ");
  vfprintf(stderr, format, args);
  fputc('\n', stderr);
}

void debug(const char *restrict format, ...) {
  va_list args;
  va_start(args, format);
  fprintf(stderr, "debug: ");
  vfprintf(stderr, format, args);
  fputc('\n', stderr);
}
