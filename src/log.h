#ifndef LOG_H
#define LOG_H

#include <stdnoreturn.h>
#include <stdarg.h>

noreturn
void panic(const char *restrict format, ...);
void error(const char *restrict format, ...);
void debug(const char *restrict format, ...);

#endif
