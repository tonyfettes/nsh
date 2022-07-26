#ifndef NSH_POOL_H
#define NSH_POOL_H

#include "nsh/stack.h"

struct pool {
  struct stack data;
  struct stack recycled;
};

#endif // NSH_POOL_H
