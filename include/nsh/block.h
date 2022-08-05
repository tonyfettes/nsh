#ifndef NSH_BLOCK_H
#define NSH_BLOCK_H

#include "nsh/statement.h"
#include "nsh/file.h"

// `compound_list` / `complete_command` / `program`
struct block {
  // Stack of `struct statement`
  struct stack statement;
  enum {
    block_index_parse = -2,
    block_index_expand = -1,
    block_index_execute = 0,
  } index;
};

void block_init(struct block *block);

void block_clear(struct block *block);

bool block_parse(struct block *block, struct parse *parse);

void block_destroy(struct block *block);

bool block_expand(struct block block, struct block *target);

bool block_execute(struct block *block, struct context *context,
                   struct stack redirect, struct pipe *pipe);

bool block_on_change(struct block *block);

#endif // NSH_BLOCK_H
