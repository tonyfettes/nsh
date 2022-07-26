#ifndef NSH_PARSE_BLOCK_H
#define NSH_PARSE_BLOCK_H

#include "nsh/parse/posix/common.h"
#include "nsh/block.h"
#include "nsh/keyword.h"

struct block_delimiter {
  enum block_delmiter_type {
    block_delimiter_operator,
    block_delimiter_keyword,
  } type;
  union {
    char operator;
    enum keyword keyword;
  };
};

bool parse_block(struct parse *parse, struct block *block,
                 struct block_delimiter delimiter);

#endif // NSH_PARSE_BLOCK_H
