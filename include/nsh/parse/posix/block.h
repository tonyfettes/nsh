#ifndef NSH_PARSE_BLOCK_H
#define NSH_PARSE_BLOCK_H

#include "nsh/parse/posix/common.h"
#include "nsh/block.h"
#include "nsh/keyword.h"

bool parse_block(struct parse *parse, struct block *block,
                 enum keyword delimiter);

#endif // NSH_PARSE_BLOCK_H
