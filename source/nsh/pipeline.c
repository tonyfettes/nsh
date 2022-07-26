#include "nsh/pipeline.h"
#include "nsh/command.h"
#include "nsh/file.h"
#include "nsh/process.h"
#include "nsh/hash.h"
#include "nsh/context.h"

void pipeline_init(struct pipeline *pipeline) {
  stack_init(&pipeline->command);
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
