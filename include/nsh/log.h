#ifndef NSH_LOG_H
#define NSH_LOG_H

enum log_level {
  log_level_trace,
  log_level_debug,
  log_level_info,
  log_level_warn,
  log_level_error,
  log_level_fatal,
};

void log_level(enum log_level level, char const *restrict format, ...);

void log_trace(char const *restrict format, ...);

void log_debug(char const *restrict format, ...);

void log_info(char const *restrict format, ...);

void log_warn(char const *restrict format, ...);

void log_error(char const *restrict format, ...);

void log_fatal(char const *restrict format, ...);

#endif // NSH_LOG_H
