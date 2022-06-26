#ifndef NSH_PARSE_COMMON_H
#define NSH_PARSE_COMMON_H

#include "nsh/parse.h"
#include <ctype.h>

bool parse_peek(struct parse *parse, char *restrict ch);

bool parse_bump(struct parse *parse);

bool parse_bump_peek(struct parse *restrict parse, char *restrict ch);

#endif // NSH_PARSE_COMMON_H
