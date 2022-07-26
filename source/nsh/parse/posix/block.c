#include "nsh/parse/posix/block.h"

#include "nsh/parse/posix/delimiter.h"
#include "nsh/parse/posix/statement.h"
#include "nsh/parse/posix/delimiter.h"

bool parse_block(struct parse *parse, struct block *block,
                 struct block_delimiter delimiter) {
  try(parse_blank(parse));
  char c;
  do {
    try(stack_alloc(&block->statement, sizeof(struct statement)));
    struct statement *statement = stack_tail(&block->statement, 0);
    statement_init(statement);
    if (!parse_statement(parse, statement)) {
      statement_destroy(statement);
      return false;
    }
    enum keyword keyword;
    if (statement_keyword(*statement, &keyword)) {
      if (delimiter.type == block_delimiter_keyword) {
        return true;
      } else {
        return false;
      }
    }
    assert(stack_bump(&block->statement, sizeof(struct statement)));
    try(parse_blank(parse));
    parse_peek(parse, &c);
    assert(c != '|' && c != '&' && c != ';');
    if (delimiter.type == block_delimiter_operator &&
        delimiter.operator == c) {
      return true;
    }
  } while (c == '\n');
  return true;
}
