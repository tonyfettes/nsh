#include "nsh/parse/posix/statement.h"

#include "nsh/parse/posix/pipeline.h"
#include "nsh/parse/posix/delimiter.h"
#include "nsh/parse/posix/command.h"

bool parse_statement(struct parse *parse, struct statement *statement) {
  char c;
  while (true) {

    try(stack_alloc(&statement->pipeline, sizeof(struct pipeline)));
    struct pipeline *pipeline = stack_tail(&statement->pipeline, 0);
    pipeline_init(pipeline);
    if (!parse_pipeline(parse, pipeline)) {
      pipeline_destroy(pipeline);
      return false;
    }
    enum keyword keyword;
    if (pipeline_keyword(*pipeline, &keyword)) {
      // `command && keyword` and `command || keyword` should always be
      // invalid.
      if (statement->pipeline.size != 0) {
        parse_error(*parse, "Unexpected keyword `%s'",
                    keyword_display(keyword));
        return false;
      }
      return true;
    }
    assert(stack_bump(&statement->pipeline, sizeof(struct pipeline)));

    try(parse_blank(parse));
    parse_peek(parse, &c);
    assert(c != '|');
    switch (c) {
    case '&':
      try(parse_bump_peek(parse, &c));
      switch (c) {
      case '&':
        try(parse_bump(parse));
        if (pipeline->condition == pipeline_bang) {
          pipeline->condition = pipeline_else;
        } else {
          pipeline->condition = pipeline_then;
        }
        try(parse_linebreak(parse));
        continue;
      default:
        statement->separator = statement_background;
        try(parse_linebreak(parse));
        return true;
      }
      break;
    case ';':
      try(parse_bump(parse));
      statement->separator = statement_sequential;
      try(parse_linebreak(parse));
      // Fall through.
    case '\n':
      return true;
    default:
      continue;
    }
  }
}
