#ifndef NSH_BUILTIN_H
#define NSH_BUILTIN_H

#include "nsh/common.h"
#include "nsh/string.h"
#include "nsh/hash.h"
#include "nsh/file.h"
#include "nsh/simple.h"

struct builtin_analysis {
  enum {
    builtin_none = 0,
    builtin_stdout = 1 << 1,
    builtin_stderr = 1 << 2,
    // Modifies the state of shell.
    builtin_extern = 1 << 4,
  } effect;
  bool valid;
  // Only used when arguments are invalid.
  int status;
};

struct builtin {
  int (*main)(struct context *context, struct simple simple);
};

extern struct hash builtin_table;

#endif // NSH_BUILTIN_H
