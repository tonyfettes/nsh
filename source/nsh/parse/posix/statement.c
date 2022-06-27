#include "nsh/parse/posix/statement.h"

#include "nsh/parse/posix/pipeline.h"
#include "nsh/parse/posix/delimiter.h"
#include "nsh/parse/posix/command.h"

bool parse_statement(struct parse *parse, struct statement *statement) {
  char c;
  while (true) {
    struct pipeline pipeline;
    pipeline_init(&pipeline);
    try(parse_pipeline(parse, &pipeline));
    try(stack_push(&statement->pipeline, &pipeline, sizeof(struct pipeline)));
    enum keyword keyword;
    if (pipeline_keyword(&pipeline, &keyword)) {
      if (statement->pipeline.size == sizeof(struct pipeline)) {
        return true;
      }
      try(stack_push(&parse->diagnosis, &(struct diagnosis) {
        .level = diagnosis_error,
      }, sizeof(struct diagnosis)));
      return false;
    }
    try(parse_peek(parse, &c));
    assert(c != '|');
    switch (c) {
    case '&':
      try(parse_bump_peek(parse, &c));
      switch (c) {
      case '&':
        try(parse_bump(parse));
        if (pipeline.condition == pipeline_bang) {
          pipeline.condition = pipeline_else;
        } else {
          pipeline.condition = pipeline_then;
        }
        try(parse_linebreak(parse));
        continue;
        break;
      default:
        statement->separator = statement_background;
        try(parse_linebreak(parse));
        return true;
        break;
      }
      break;
    case ';':
      try(parse_bump(parse));
      statement->separator = statement_sequential;
      try(parse_linebreak(parse));
      // Fall through.
    case '\n':
      return true;
      break;
    default:
      continue;
      break;
    }
  }
}
