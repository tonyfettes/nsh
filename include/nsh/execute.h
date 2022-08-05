#ifndef NSH_EXECUTE_H
#define NSH_EXECUTE_H

#include "nsh/stack.h"
#include "nsh/pipeline.h"
#include "nsh/loop.h"
#include "nsh/block.h"
#include "nsh/job.h"

enum execute_flags {
  execute_subshell = 1 << 0,
};

bool execute_pipeline(struct stack *job, struct pipeline pipeline,
                      struct pipe *pipe);

bool execute_for_in(struct stack *job, struct for_in for_in,
                    struct pipe pipe);

bool execute_block(struct stack *job, struct block block,
                   struct pipe pipe);

bool execute_statement(struct stack *job, struct statement statement,
                       struct pipe pipe);

#endif // NSH_EXECUTE_H
