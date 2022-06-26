#ifndef NSH_EXECUTE_SIMPLE_H
#define NSH_EXECUTE_SIMPLE_H

#include "nsh/simple.h"
#include "nsh/redirect.h"

bool execute_simple(struct simple const *simple,
                    struct stack const *redirect);

#endif // NSH_EXECUTE_SIMPLE_H
