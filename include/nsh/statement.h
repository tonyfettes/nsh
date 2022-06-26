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
  // Stack of `struct pipeline`.
  struct stack pipeline;
};

void statement_init(struct statement *statement);

bool statement_keyword(struct statement const *statement,
                       enum keyword *keyword);

void statement_destroy(struct statement *statement);

#endif // NSH_STATEMENT_H
