#ifndef NSH_LOG_H
#define NSH_LOG_H

#include "nsh/location.h"
#include <stdio.h>
#include <stdbool.h>
#include "nsh/stack.h"

enum log_level {
  log_level_trace,
  log_level_debug,
  log_level_info,
  log_level_warn,
  log_level_error,
  log_level_fatal,
};

__attribute__((format(printf, 3, 4)))
void log_level_at(struct location location, enum log_level level,
                  char const *restrict format, ...);

#define log_level(level, ...)                                          \
  log_level_at(locate(), log_level_##level, __VA_ARGS__)

#define log_trace(...) log_level(trace, __VA_ARGS__)
#define log_debug(...) log_level(debug, __VA_ARGS__)
#define log_info(...)  log_level(info,  __VA_ARGS__)
#define log_warn(...)  log_level(warn,  __VA_ARGS__)
#define log_error(...) log_level(error, __VA_ARGS__)
#define log_fatal(...) log_level(fatal, __VA_ARGS__)

struct log_redirect {
  enum log_redirect_type {
    log_redirect_file_pointer,
    log_redirect_file_descriptor,
    log_redirect_memory,
  } type;
  union {
    FILE *file_pointer;
    int file_descriptor;
    struct stack memory;
  };
};

bool log_redirect(enum log_level level, struct log_redirect redirect);

#endif // NSH_LOG_H
