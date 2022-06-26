#include "nsh/parse/block.h"

#include "nsh/parse/delimiter.h"
#include "nsh/parse/statement.h"

bool parse_block(struct parse *parse, struct block *block,
                 enum keyword delimiter) {
  char c;
  do {
    try(stack_alloc(&block->statement, sizeof(struct statement)));
    struct statement *statement = stack_tail(&block->statement, 0);
    statement_init(statement);
    try(parse_statement(parse, statement));
    enum keyword keyword;
    if (statement_keyword(statement, &keyword) &&
        keyword == delimiter) {
      return true;
    }
    assert(stack_bump(&block->statement, sizeof(struct statement)));
    try(parse_peek(parse, &c));
    assert(c != '|' && c != '&' && c != ';');
  } while (c == '\n');
  return true;
}
