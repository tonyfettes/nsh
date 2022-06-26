#ifndef NSH_BLOCK_H
#define NSH_BLOCK_H

#include "nsh/statement.h"

// `compound_list`
struct block {
  // Stack of `struct statement`
  struct stack statement;
};

void block_init(struct block *block);

void block_clear(struct block *block);

void block_destroy(struct block *block);

#endif /* NSH_BLOCK_H */
