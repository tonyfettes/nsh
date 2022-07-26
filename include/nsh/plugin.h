#ifndef NSH_PLUGIN_H
#define NSH_PLUGIN_H

#include "nsh/string.h"

struct plugin {
  char const *name;
  void *data;
  bool (*init)();
  bool (*destroy)(void *);
};

#endif // NSH_PLUGIN_H
