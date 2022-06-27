#include "nsh/parse/posix/branch.h"

#include "delimiter.h"
#include "statement.h"
#include "pattern.h"
#include "word.h"
#include "linebreak.h"

static bool parse_elif_part(struct context *restrict context,
                            struct if_branch *restrict if_branch) {
  if (!parse_expect(context, "elif")) {
    return false;
  }
  if (!parse_delimiter(context)) {
    return false;
  }
  if (!parse_block(context, &if_branch->condition)) {
    return false;
  }
  parse_delimiter(context);
  if (!parse_expect(context, "then")) {
    return false;
  }
  if (!parse_delimiter(context)) {
    return false;
  }
  if (!parse_block(context, &if_branch->body)) {
    return false;
  }
  return true;
}

static bool parse_else_part(struct context *restrict context,
                            struct block *restrict else_body) {
  if (!parse_expect(context, "else")) {
    return false;
  }
  if (!parse_delimiter(context)) {
    return false;
  }
  return parse_block(context, else_body);
}

static bool parse_fi(struct context *restrict context) {
  return parse_expect(context, "fi");
}

extern bool parse_if_clause(struct context *restrict context,
                            struct if_clause *restrict if_clause) {
  if (!parse_expect(context, "if")) {
    return false;
  }
  parse_delimiter(context);
  if (!parse_block(context, &if_clause->if_branch.condition)) {
    return false;
  }
  parse_delimiter(context);
  if (!parse_expect(context, "then")) {
    return false;
  }
  parse_delimiter(context);
  if (!parse_block(context, &if_clause->if_branch.body)) {
    return false;
  }
  while (true) {
    if (parse_fi(context)) {
      return true;
    }
    if (parse_else_part(context, &if_clause->else_body)) {
      return parse_fi(context);
    }
    struct if_branch elif_branch;
    if (!parse_elif_part(context, &elif_branch)) {
      return false;
    }
    if (!stack_push(&if_clause->elif_branch, &elif_branch,
                    sizeof(elif_branch))) {
      return false;
    }
  }
}

static bool parse_case_item(struct context *restrict context,
                            struct case_item *restrict case_item) {
  parse_expect(context, "(");
  parse_delimiter(context);
  regex_t regex;
  if (!parse_pattern(context, &regex)) {
    return false;
  }
  parse_delimiter(context);
  stack_push(&case_item->pattern, &regex, sizeof(regex));
  while (true) {
    if (!parse_pattern(context, &regex)) {
      break;
    }
    parse_delimiter(context);
    stack_push(&case_item->pattern, &regex, sizeof(regex));
  }
  if (!parse_expect(context, ")")) {
    return false;
  }
  parse_delimiter(context);
  parse_block(context, &case_item->block);
  return true;
}

extern bool parse_case(struct context *restrict context,
                       struct case_clause *restrict case_clause) {
  if (!parse_expect(context, "case")) {
    return false;
  }
  parse_delimiter(context);
  if (!parse_word(context, &case_clause->word)) {
    return false;
  }
  parse_linebreak(context);
  parse_expect(context, "in");
  parse_linebreak(context);

  while (true) {
    struct case_item case_item;
    if (!parse_case_item(context, &case_item)) {
      break;
    }
    parse_linebreak(context);
    if (!parse_expect(context, ";;")) {
      break;
    }
    parse_linebreak(context);
  }

  if (!parse_expect(context, "esac")) {
    return false;
  }
  return true;
}
