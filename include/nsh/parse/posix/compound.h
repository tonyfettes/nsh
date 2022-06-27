#ifndef NSH_PARSE_COMPOUND_H
#define NSH_PARSE_COMPOUND_H

#include "nsh/parse/posix/common.h"
#include "nsh/command.h"

bool parse_subshell(struct parse *parse, struct subshell *subshell);

bool parse_brace(struct parse *parse, struct brace *subshell);

#endif // NSH_PARSE_COMPOUND_H
