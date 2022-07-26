#include "nsh/execute/statement.h"
#include "nsh/execute/pipeline.h"
#include <sys/wait.h>
#include "nsh/job.h"
#include "nsh/context.h"

bool execute_statement(struct stack *job, struct statement statement) {
  struct pipeline *head = stack_head(&statement.pipeline, 0);
  struct pipeline *tail = stack_tail(&statement.pipeline, 0);
  for (struct pipeline *pipeline = head; pipeline != tail; pipeline++) {
    if (!execute_pipeline(job, *pipeline)) {
      return false;
    }
  }
}

bool execute_statement(int *status, struct statement *statement) {
  struct pipeline const *pipeline = statement->pipeline.data;
  size_t size = statement->pipeline.size / sizeof(struct pipeline);
  assert(size >= 1);
  if (statement->separator == statement_background) {
    struct job job;
    try(job_init(&job, statement));
    try(execute_pipeline(&job.process, &pipeline[0]));
    try(stack_push(&context.job, &job, sizeof(struct job)));
    *status = 0;
    return true;
  }
  struct stack process;
  stack_init(&process);
  for (size_t i = 0; i < size; i++) {
    try(execute_pipeline(&process, &pipeline[i]));
    struct process *last = stack_tail(&process, sizeof(struct process));
    try(process_wait(*last, status, 0));
    if (WIFEXITED(*status)) {
      if ((WEXITSTATUS(*status) == 0 &&
           pipeline[i].condition == pipeline_then) ||
          (WEXITSTATUS(*status) != 0 &&
           pipeline[i].condition == pipeline_else)) {
        continue;
      } else {
        break;
      }
    } else if (WIFCONTINUED(*status)) {
      todo("Handling case where process is continued");
    } else if (WIFSIGNALED(*status)) {
      todo("Handling case where process is signaled");
    } else if (WIFSTOPPED(*status)) {
      todo("Handling case where process is stopped");
    }
  }
  return true;
}
