#include "nsh/block.h"

void block_init(struct block *block) {
  stack_init(&block->statement);
}

void block_clear(struct block *block) {
  stack_clear(&block->statement);
}

void block_destroy(struct block *block) {
  stack_destroy(&block->statement);
}
