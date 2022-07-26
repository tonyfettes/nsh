#include "nsh/parse/posix/pipeline.h"

#include "nsh/parse/posix/delimiter.h"
#include "nsh/parse/posix/command.h"

static bool parse_bang(struct parse *parse, struct pipeline *pipeline) {
  char c;
  parse_peek(parse, &c);
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
    try(stack_alloc(&pipeline->command, sizeof(struct command)));
    struct command *command = stack_tail(&pipeline->command, 0);
    command_init(command);
    if (!parse_command(parse, command)) {
      command_destroy(command);
      return false;
    }
    if (command->type == command_keyword) {
      // `command | keyword` should always be invalid.
      if (pipeline->command.size != 0) {
        parse_error(*parse, "Unexpected keyword `%s'",
                    keyword_display(command->keyword));
        return false;
      }
      // If command is a keyword, then parse this pipeline as keyword.
      return true;
    }
    assert(stack_bump(&pipeline->command, sizeof(struct command)));

    try(parse_blank(parse));
    parse_peek(parse, &c);
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
