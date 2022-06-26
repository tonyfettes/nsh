#include "nsh/statement.h"

void statement_init(struct statement *statement) {
  stack_init(&statement->pipeline);
}

bool statement_keyword(struct statement const *statement,
                       enum keyword *keyword) {
  if (statement->pipeline.size != sizeof(struct pipeline)) {
    return false;
  }
  struct pipeline *pipeline = statement->pipeline.data;
  return pipeline_keyword(pipeline, keyword);
}

void statement_destroy(struct statement *statement) {
  stack_destroy(&statement->pipeline);
}