#include "nsh/pipeline.h"
#include "nsh/command.h"
#include "nsh/file.h"
#include "nsh/process.h"
#include "nsh/hash.h"
#include "nsh/context.h"
#include "nsh/execute.h"

void pipeline_init(struct pipeline *pipeline) {
  stack_init(&pipeline->command);
}

void pipeline_clear(struct pipeline *pipeline) {
}

bool pipeline_keyword(struct pipeline pipeline,
                      enum keyword *keyword) {
  if (pipeline.command.size != sizeof(struct command)) {
    return false;
  }
  struct command *command = stack_head(&pipeline.command, 0);
  if (command->type != command_keyword) {
    return false;
  }
  *keyword = command->keyword;
  return true;
}

void pipeline_destroy(struct pipeline *pipeline) {
  stack_destroy(&pipeline->command);
}

bool pipeline_expand(struct pipeline pipeline, struct pipeline *target) {
  assert(!pipeline.expanded);
  struct command *head = stack_head(&pipeline.command, 0);
  struct command *tail = stack_tail(&pipeline.command, 0);
  for (struct command *in = head; in != tail; in++) {
    if (!stack_alloc(&target->command, sizeof(struct command))) {
      return false;
    }
    struct command *out = stack_tail(&target->command, 0);
    command_init(out);
    if (!command_expand(*in, out)) {
      command_destroy(out);
      return false;
    }
    assert(stack_bump(&target->command, sizeof(struct command)));
  }
  return true;
}

bool pipeline_execute(struct pipeline pipeline, struct context *context,
                      struct pipe *pipe) {
  stack_size_t pipeline_size;
  pipeline_size = pipeline.command.size / sizeof(struct pipeline);
  assert(pipeline_size != 0);
  struct command *head = stack_head(&pipeline.command, 0);
  struct command *tail = stack_tail(&pipeline.command, 0);
  if (pipeline_size == 1) {
    if (!command_execute(*head, context, pipe)) {
      return false;
    }
  }
  struct command *last = tail - 1;
  struct file *relay = pipe->read;
  for (struct command *command = head; command < last; command++) {
    struct pipe temp;
    if (!pipe_open(&temp)) {
      return false;
    }
    struct pipe pipe_in;
    pipe_in.read = relay;
    pipe_in.write = temp.write;
    relay = pipe_in.read;
    struct context isolation;
    if (!context_fork(&isolation, *context)) {
      return false;
    }
    if (!command_execute(*command, &isolation, &pipe_in)) {
      return false;
    }
  }
  struct pipe last_pipe = {
    .read = relay,
    .write = pipe->write,
  };
  struct context isolation;
  if (!context_fork(&isolation, *context)) {
    return false;
  }
  return command_execute(*last, &isolation, &last_pipe);
}
