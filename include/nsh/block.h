#ifndef NSH_BLOCK_H
#define NSH_BLOCK_H

#include "nsh/statement.h"

// `compound_list` / `complete_command` / `program`
struct block {
  // Stack of `struct statement`
  struct stack statement;
};

void block_init(struct block *block);

void block_clear(struct block *block);

bool block_parse(struct block *block, struct parse *parse);

void block_destroy(struct block *block);

#endif // NSH_BLOCK_H
