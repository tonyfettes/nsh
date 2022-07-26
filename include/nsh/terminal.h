#ifndef NSH_TERMINAL_H
#define NSH_TERMINAL_H

#include "nsh/common.h"
#include "nsh/process.h"
#include <termios.h>
#include <unistd.h>

enum modifier {
  modifier_none = 0,
  modifier_ctrl = 1 << 0,
  modifier_alt = 1 << 1,
  modifier_meta = 1 << 2,
};

enum color {
  color_black = 0,
  color_red = 1,
  color_green = 2,
  color_brown = 3,
  color_blue = 4,
  color_magenta = 5,
  color_cyan = 6,
  color_white = 7,
};

enum emphasis {
  emphasis_normal = 0,
  emphasis_bold = 1 << 0,
  emphasis_underline = 1 << 1,
  emphasis_italic = 1 << 2,
};

struct style {
  enum color foreground;
  enum color background;
  unsigned int emphasis;
};

struct terminal {
  bool available;
  int fd;
  struct style style;
  enum terminal_mode {
    terminal_raw,
    terminal_normal,
  } mode;
  struct termios termios[2];
  struct {
    unsigned int height;
    unsigned int width;
  } geometry;
};

bool terminal_init(struct terminal *terminal);

bool terminal_select(struct terminal *terminal,
                     enum terminal_mode mode);

bool terminal_refresh(struct terminal *terminal);

bool terminal_destroy(struct terminal *terminal);

#endif // NSH_TERMINAL_H
