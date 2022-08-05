#include "nsh/statement.h"
#include "nsh/context.h"
#include <sys/wait.h>

void statement_init(struct statement *statement) {
  stack_init(&statement->pipeline);
  statement->index = -1;
}

bool statement_keyword(struct statement statement,
                       enum keyword *keyword) {
  if (statement.pipeline.size != sizeof(struct pipeline)) {
    return false;
  }
  struct pipeline *pipeline = stack_head(&statement.pipeline, 0);
  return pipeline_keyword(*pipeline, keyword);
}

void statement_destroy(struct statement *statement) {
  stack_destroy(&statement->pipeline);
}

bool statement_expand(struct statement statement,
                      struct statement *target) {
  struct pipeline *head = stack_head(&statement.pipeline, 0);
  struct pipeline *tail = stack_tail(&statement.pipeline, 0);
  for (struct pipeline *in = head; in != tail; in++) {
    if (!stack_alloc(&target->pipeline, sizeof(struct pipeline))) {
      return false;
    }
    struct pipeline *out = stack_tail(&target->pipeline, 0);
    pipeline_init(out);
    if (!pipeline_expand(*in, out)) {
      pipeline_destroy(out);
      return false;
    }
    assert(stack_bump(&target->pipeline, sizeof(struct pipeline)));
  }
  return true;
}

bool statement_execute(struct statement *statement,
                       struct context *context, struct pipe *pipe) {
  assert(statement->pipeline.size / sizeof(struct pipeline) >= 1);
  assert(statement->index >= statement_index_execute);
  struct pipeline *pipeline;
  pipeline = stack_head(&statement->pipeline,
                        statement->index * sizeof(struct pipeline));
  if (!stack_alloc(&context->job, sizeof(struct job))) {
    return false;
  }
  struct job *job = stack_tail(&context->job, 0);
  job_init(job);
  job->id = context->job.size / sizeof(struct job);
  if (!pipeline_execute(*pipeline, context, pipe)) {
    job_destroy(job);
    return false;
  }
  statement->index++;
  if (job->group.member.size == 0) {
    job_destroy(job);
  } else {
    assert(stack_bump(&context->job, sizeof(struct job)));
  }
  return true;
}

static bool on_change_next(struct pipeline *pipeline,
                           struct job_list *job_list) {
  if (!job_list_alloc(job_list)) {
    return false;
  }
  struct job *job = job_list_tail(job_list);
  job_init(job);
  struct pipe pipe = {
    .read = NULL,
    .write = NULL,
  };
  if (context.option.job.control) {
    pipe.read = &context.file.sink;
  }
  if (!pipeline_execute(*pipeline, job, &pipe, 0)) {
    return false;
  }
  assert(job_list_bump(job_list));
  return true;
}

bool statement_update(struct statement *statement, int status) {
  stack_size_t index = statement->index;
  stack_size_t size = statement->pipeline.size /
                      sizeof(struct pipeline);
  assert(index < size);
  struct pipeline *next;
  next = stack_head(&statement->pipeline,
                    index * sizeof(struct pipeline));
  struct pipeline *prev = next - 1;
  switch (prev->condition) {
  case pipeline_then:
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
      on_change_next(next, &context.job_list);
      index++;
    } else {
      index = size;
    }
    break;
  case pipeline_else:
    if (!WIFEXITED(status) ||
        (WIFEXITED(status) && WEXITSTATUS(status))) {
      if (!on_change_next(next, &context.job_list)) {
        return false;
      }
      index++;
    } else {
      index = size;
    }
    break;
  case pipeline_none:
  case pipeline_bang:
    assert(prev->condition != pipeline_none &&
           prev->condition != pipeline_bang);
    break;
  }
  return true;
}
