#ifndef NSH_HIGHLIGHT_H
#define NSH_HIGHLIGHT_H

#include "nsh/location.h"
#include "nsh/common.h"

struct highlight {
  struct location location;
  size_t size;
};

#endif // NSH_HIGHLIGHT_H
