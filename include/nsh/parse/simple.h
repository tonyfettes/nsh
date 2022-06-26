#ifndef NSH_PARSE_SIMPLE_H
#define NSH_PARSE_SIMPLE_H

#include "nsh/parse/common.h"
#include "nsh/simple.h"
#include "nsh/redirect.h"

bool parse_simple(struct parse *parse, struct simple *simple,
                  struct stack *redirect_stack);

#endif // NSH_PARSE_SIMPLE_H
