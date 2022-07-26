#ifndef NSH_PARSE_POSIX_H
#define NSH_PARSE_POSIX_H

#include "nsh/common.h"

bool parse_block(struct parse *parse, struct block *block,
                 enum keyword delimiter);

#endif // NSH_PARSE_POSIX_H
