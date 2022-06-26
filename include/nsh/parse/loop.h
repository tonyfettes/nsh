#ifndef NSH_PARSE_LOOP_H
#define NSH_PARSE_LOOP_H

#include "nsh/parse/common.h"
#include "nsh/loop.h"

bool parse_while_loop(struct parse *parse,
                      struct while_loop *while_loop);

bool parse_until_loop(struct parse *parse,
                      struct until_loop *until_loop);

bool parse_for_loop(struct parse *parse, struct for_loop *for_loop);

#endif // NSH_PARSE_LOOP_H
