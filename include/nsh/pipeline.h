#ifndef NSH_PIPELINE_H
#define NSH_PIPELINE_H

#include "nsh/stack.h"
#include "nsh/keyword.h"
#include "nsh/file.h"

struct pipeline {
  // Condition operator that suffixes (`&&` and `||`) or prefixes (`!`)
  // the pipeline.
  enum {
    pipeline_then, // `... &&` or `! ... ||`
    pipeline_else, // `... ||` or `! ... &&`
    pipeline_none, // `...`
    pipeline_bang, // `! ...`
  } condition;
  bool expanded;
  struct stack command;
};

void pipeline_init(struct pipeline *pipeline);

bool pipeline_keyword(struct pipeline pipeline, enum keyword *keyword);

void pipeline_destroy(struct pipeline *pipeline);

bool pipeline_display(struct pipeline const *pipeline,
                      struct string *target);

bool pipeline_expand(struct pipeline pipeline, struct pipeline *target);

bool pipeline_execute(struct pipeline pipeline, struct context *context,
                      struct pipe *pipe);

#endif // NSH_PIPELINE_H
