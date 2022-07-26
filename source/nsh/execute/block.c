#include "nsh/execute.h"

bool execute_block(struct stack *job, struct block block) {
  struct statement *head = stack_head(&block.statement, 0);
  struct statement *tail = stack_tail(&block.statement, 0);
  for (struct statement *stmt = head; stmt != tail; stmt++) {
    if (!execute_statement(job, *stmt)) {
      return false;
    }
  }
  return true;
}
