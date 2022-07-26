#include "nsh/terminal.h"

#include <string.h>
#include <stdio.h>
#include "nsh/string.h"

bool terminal_select(struct terminal *terminal,
                     enum terminal_mode mode) {
  assert(terminal->available);
  if (mode == terminal->mode) {
    return true;
  }
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH,
                &terminal->termios[terminal_raw]) == -1) {
    log_error("Failed to set terminal attributes: %s.",
              strerror(errno));
    return false;
  }
  return true;
}

bool terminal_init(struct terminal *terminal) {
  terminal->fd = STDIN_FILENO;
  terminal->available = false;
  if (isatty(STDIN_FILENO) == 0) {
    assert(errno != EBADF);
    return true;
  }
  if (tcgetattr(STDIN_FILENO, &terminal->termios[terminal_normal]) == -1) {
    log_error("Failed to get terminal attributes: %s.",
              strerror(errno));
    return false;
  }
  terminal->termios[terminal_raw] = terminal->termios[terminal_normal];
  cfmakeraw(&terminal->termios[terminal_raw]);
  terminal->available = true;
  terminal_select(terminal, terminal_raw);
  // Set stdout to be unbuffered.
  // setbuf(stdout, NULL);
  // Enter insert mode.
  // if (fputs("\x1b[4h", stdout) < 0) {
  //   log_error("Failed to enter insert mode.");
  //   return false;
  // }
  return true;
}

bool terminal_printf(struct terminal *terminal, struct style style,
                     char const *restrict format, ...) {
  static char const *const foreground[] = {
    [color_black] = "\x1b[30m",
    [color_red] = "\x1b[31m",
    [color_green] = "\x1b[32m",
    [color_brown] = "\x1b[33m",
    [color_blue] = "\x1b[34m",
    [color_magenta] = "\x1b[35m",
    [color_cyan] = "\x1b[36m",
    [color_white] = "\x1b[37m",
  };
  static char const *const background[] = {
    [color_black] = "\x1b[40m",
    [color_red] = "\x1b[41m",
    [color_green] = "\x1b[42m",
    [color_brown] = "\x1b[44m",
    [color_blue] = "\x1b[44m",
    [color_magenta] = "\x1b[45m",
    [color_cyan] = "\x1b[46m",
    [color_white] = "\x1b[47m",
  };
  struct string setter;
  string_init(&setter);
  if (style.foreground != terminal->style.foreground) {
    if (!string_puts(&setter, foreground[style.foreground])) {
      string_destroy(&setter);
      return false;
    }
  }
  if (style.background != terminal->style.background) {
    if (!string_puts(&setter, background[style.background])) {
      string_destroy(&setter);
      return false;
    }
  }
  va_list args;
  va_start(args, format);
  bool ok = string_vprintf(&setter, format, args);
  va_end(args);
  if (!ok) {
    return false;
  }
  if (fputs(string_data(&setter), stdout) < 0) {
    return false;
  }
  return true;
}

bool terminal_refresh(struct terminal *terminal) {
  ignore(terminal);
  if (fflush(stdout) == EOF) {
    throw_system(errno);
    return false;
  }
  return true;
}

bool terminal_destroy(struct terminal *terminal) {
  bool ok = true;
  // `setvbuf()` *may* set errno on failure.
  // errno = 0;
  // if (setvbuf(stdout, NULL, _IOLBF, 0) != 0) {
  //   ok = false;
  //   log_error("Failed to restore to line buffered mode%s%s.",
  //             errno ? ": " : "", errno ? strerror(errno) : "");
  // }
  if (!terminal_select(terminal, terminal_normal)) {
    ok = false;
  }
  if (!ok) {
    return false; 
  }
  return ok;
}
