#ifndef NSH_STATEMENT_H
#define NSH_STATEMENT_H

#include "nsh/pipeline.h"
#include "nsh/keyword.h"

// `and_or`
struct statement {
  enum {
    statement_background, // `&`
    statement_sequential, // `;`
  } separator;
  struct stack pipeline;
};

void statement_init(struct statement *statement);

bool statement_keyword(struct statement statement,
                       enum keyword *keyword);

bool statement_parse(struct statement *statement, struct parse *parse);

void statement_destroy(struct statement *statement);

#endif /* NSH_STATEMENT_H */
