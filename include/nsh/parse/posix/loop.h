#ifndef NSH_PARSE_LOOP_H
#define NSH_PARSE_LOOP_H

#include "nsh/parse/posix/common.h"
#include "nsh/loop.h"

bool parse_loop(struct parse *parse, struct loop *loop);

bool parse_for_in(struct parse *parse, struct for_in *for_in);

#endif // NSH_PARSE_LOOP_H
