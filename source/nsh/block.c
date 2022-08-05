#include "nsh/block.h"
#include "nsh/context.h"

void block_init(struct block *block) {
  stack_init(&block->statement);
  block->index = -1;
}

static void block_content_clear(struct block *block) {
  struct statement *head = stack_head(&block->statement, 0);
  struct statement *tail = stack_tail(&block->statement, 0);
  for (struct statement *stmt = head; stmt != tail; stmt++) {
    statement_destroy(stmt);
  }
}

void block_clear(struct block *block) {
  block_content_clear(block);
  stack_clear(&block->statement);
}

void block_destroy(struct block *block) {
  block_content_clear(block);
  stack_destroy(&block->statement);
}

bool block_expand(struct block block, struct block *target) {
  assert(block.index == block_index_parse);
  block_clear(target);
  struct statement *head = stack_head(&block.statement, 0);
  struct statement *tail = stack_tail(&block.statement, 0);
  for (struct statement *in = head; in != tail; in++) {
    if (!stack_alloc(&target->statement, sizeof(struct statement))) {
      return false;
    }
    struct statement *out = stack_tail(&target->statement, 0);
    statement_init(out);
    if (!statement_expand(*in, out)) {
      statement_destroy(out);
      return false;
    }
    assert(stack_bump(&target->statement, sizeof(struct statement)));
  }
  target->index = block_index_expand;
  return true;
}

bool block_execute(struct block *block, struct context *context,
                   struct stack redirect, struct pipe *pipe) {
  assert(block->index == block_index_expand);
  stack_size_t index = 0;
  struct statement *head = stack_head(&block->statement, 0);
  struct statement *tail = stack_tail(&block->statement, 0);
  for (struct statement *stmt = head; stmt != tail; stmt++, index++) {
    if (!statement_execute(stmt, context, pipe)) {
      return false;
    }
    if (stmt->separator == statement_sequential) {
      break;
    }
  }
  block->index = index;
  return true;
}

bool block_update(struct block *block) {
  stack_size_t index = block->index;
  stack_size_t size = block->statement.size / sizeof(struct statement);
  assert(index < size);
  struct statement *tail;
  tail = stack_tail(&block->statement, 0);
  struct statement *next;
  next = stack_head(&block->statement,
                    index * sizeof(struct statement));
  for (struct statement *stmt = next; stmt != tail; stmt++, index++) {
    struct pipe pipe = {
      .read = NULL,
      .write = NULL,
    };
    if (!statement_execute(stmt, &context.job_list, &pipe, 0)) {
      return false;
    }
    if (stmt->separator == statement_sequential) {
      break;
    }
  }
  block->index = index;
  return true;
}
