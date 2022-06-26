#ifndef NSH_PIPELINE_H
#define NSH_PIPELINE_H

#include "nsh/stack.h"
#include "nsh/keyword.h"

struct pipeline {
  // Condition operator that suffixes (`&&` and `||`) or prefixes (`!`)
  // the pipeline.
  enum {
    pipeline_then, // `... &&` or `! ... ||`
    pipeline_else, // `... ||` or `! ... &&`
    pipeline_none, // `...`
    pipeline_bang, // `! ...`
  } condition;
  // Stack of `struct command`.
  struct stack command;
};

void pipeline_init(struct pipeline *pipeline);

bool pipeline_keyword(struct pipeline const *pipeline,
                      enum keyword *keyword);

void pipeline_destroy(struct pipeline *pipeline);

#endif // NSH_PIPELINE_H
