#ifndef NSH_STATEMENT_H
#define NSH_STATEMENT_H

#include "nsh/pipeline.h"
#include "nsh/keyword.h"
#include "nsh/file.h"
#include "nsh/job.h"

// `and_or`
struct statement {
  enum {
    statement_background, // `&`
    statement_sequential, // `;`
  } separator;
  struct stack pipeline;
  enum {
    statement_index_parse = -2,
    statement_index_expand = -1,
    statement_index_execute = 0,
  } index;
};

void statement_init(struct statement *statement);

bool statement_keyword(struct statement statement,
                       enum keyword *keyword);

bool statement_parse(struct statement *statement, struct parse *parse);

void statement_destroy(struct statement *statement);

bool statement_expand(struct statement statement,
                      struct statement *target);

bool statement_execute(struct statement *statement,
                       struct context *context, struct pipe *pipe);

bool statement_on_change(struct statement *statement, int status);

#endif /* NSH_STATEMENT_H */
