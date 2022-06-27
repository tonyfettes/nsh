#include "nsh/parse/posix/pipeline.h"

#include "nsh/parse/posix/delimiter.h"
#include "nsh/parse/posix/command.h"

static bool parse_bang(struct parse *parse, struct pipeline *pipeline) {
  char c;
  try(parse_peek(parse, &c));
  if (c == '!') {
    pipeline->condition = pipeline_bang;
    try(parse_bump(parse));
  } else {
    pipeline->condition = pipeline_none;
  }
  return true;
}

bool parse_pipeline(struct parse *parse, struct pipeline *pipeline) {
  try(parse_bang(parse, pipeline));
  try(parse_blank(parse));
  char c;
  do {
    struct command command;
    command_init(&command);
    try(parse_command(parse, &command));
    if (command.type == command_keyword) {
      if (pipeline->command.size != 0) {
        try(stack_push(&parse->diagnosis, &(struct diagnosis) {
          .level = diagnosis_error,
          .type = diagnosis_unexpected,
          .location = parse->location,
        }, sizeof(struct diagnosis)));
        return false;
      }
      return true;
    }
    try(stack_push(&pipeline->command, &command, sizeof(command)));
    try(parse_blank(parse));
    try(parse_peek(parse, &c));
    // Neither `|` nor `||`.
    if (c != '|') {
      break;
    }
    // `|`
    try(parse_bump_peek(parse, &c));
    if (c != '|') {
      try(parse_linebreak(parse));
      continue;
    }
    // `||`
    try(parse_bump(parse));
    if (pipeline->condition == pipeline_bang) {
      pipeline->condition = pipeline_then;
    } else {
      pipeline->condition = pipeline_else;
    }
    try(parse_linebreak(parse));
    break;
  } while (true);
  return true;
}
