#include "nsh/parse/posix/command.h"

#include "nsh/context.h"
#include "nsh/parse/posix/delimiter.h"
#include "nsh/parse/posix/block.h"
#include "nsh/parse/posix/component.h"
#include "nsh/parse/posix/simple.h"
#include "nsh/parse/posix/segment.h"
#include "nsh/parse/posix/loop.h"
#include "nsh/parse/posix/function.h"
#include "nsh/parse/posix/compound.h"

static bool handle_function_or_simple(struct parse *parse,
                                      struct command *command,
                                      struct component *component) {
  try(parse_blank(parse));
  char c;
  parse_peek(parse, &c);
  if (c == '(') {
    try(parse_blank(parse));
    parse_peek(parse, &c);
    if (c != ')') {
      parse_error(*parse, "Expect ')' after '(', but got %c", c);
      // throw(parse, invalid_syntax);
      return false;
    }
    try(command_select(command, command_function));
    command->function->name = component->word;
    component->type = component_none;
    try(parse_blank(parse));
    try(parse_command(parse, &command->function->body));
  } else {
    try(command_select(command, command_simple));
    try(stack_push(&command->simple.argument, &component->word,
                   sizeof(struct word)));
    component->type = component_none;
    try(parse_simple(parse, &command->simple, &command->redirect));
  }
  return true;
}

static bool handle_alias(struct parse *parse, struct command *command,
                         struct component *component) {
  struct segment *seg = stack_head(&component->word.segment, 0);
  for (struct track *track = stack_head(&parse->track, 0),
       *tail = stack_tail(&parse->track, 0); track != tail; track++) {
    if (string_equal(seg->plain, track->name)) {
      return handle_function_or_simple(parse, command, component);
    }
  }
  try(stack_alloc(&parse->track, sizeof(struct track)));
  struct track *track = stack_tail(&parse->track, 0);
  track_init(track);
  if (alias_table_find(context.alias_table, seg->plain,
                       &track->value)) {
    string_swap(&track->name, &seg->plain);
    word_destroy(&component->word);
    component->type = component_none;
    assert(stack_bump(&parse->track, sizeof(struct track)));
    if (!parse_blank(parse)) {
      track_destroy(track);
      return false;
    }
    if (!parse_command(parse, command)) {
      track_destroy(track);
      return false;
    }
    return true;
  }
  if (catch(hash, not_exist)) {
    return handle_function_or_simple(parse, command, component);
  }
  return false;
}

static bool handle_keyword(struct parse *parse, struct command *command,
                           struct component *component) {
  struct segment *seg = stack_head(&component->word.segment, 0);
  enum keyword keyword = keyword_from(seg->plain);
  switch (keyword) {
  case keyword_brace_open:
    try(command_select(command, command_brace));
    try(parse_linebreak(parse));
    try(parse_brace(parse, &command->brace));
    try(parse_blank(parse));
    try(parse_redirect_list(parse, &command->redirect));
    return true;
  case keyword_while:
  case keyword_until:
    try(command_select(command, command_loop));
    if (keyword == keyword_while) {
      command->loop.type = loop_while;
    } else {
      command->loop.type = loop_until;
    }
    try(parse_linebreak(parse));
    try(parse_loop(parse, &command->loop));
    try(parse_blank(parse));
    try(parse_redirect_list(parse, &command->redirect));
    return true;
  case keyword_for:
    try(command_select(command, command_for_in));
    try(parse_linebreak(parse));
    try(parse_for_in(parse, &command->for_in));
    try(parse_blank(parse));
    try(parse_redirect_list(parse, &command->redirect));
    return true;
  case keyword_none:
    break;
  default:
    try(command_select(command, command_keyword));
    command->keyword = keyword;
    return true;
  }
  return handle_alias(parse, command, component);
}

static bool handle_multiple_segment(struct parse *parse,
                                    struct command *command,
                                    struct component *component) {
  stack_size_t seg_cnt = component->word.segment.size;
  seg_cnt /= sizeof(struct segment);
  assert(seg_cnt != 0);
  struct segment *seg = stack_head(&component->word.segment, 0);
  if (component->word.quoted || seg_cnt >= 1 ||
      seg->type != segment_plain) {
    return handle_function_or_simple(parse, command, component);
  }
  return handle_keyword(parse, command, component);
}

static bool parse_command_component(struct parse *parse,
                                    struct command *command,
                                    struct component *component) {
  try(parse_component(parse, component, true));
  switch (component->type) {
  case component_none:
    assert(component->type != component_none);
    break;
  case component_redirect:
    try(command_select(command, command_simple));
    try(stack_push(&command->redirect, &component->redirect,
                   sizeof(struct redirect)));
    component->type = component_none;
    try(parse_blank(parse));
    try(parse_simple(parse, &command->simple, &command->redirect));
    return true;
  case component_environment:
    try(command_select(command, command_simple));
    try(stack_push(&command->simple.environment,
                   &component->environment,
                   sizeof(struct environment)));
    component->type = component_none;
    try(parse_blank(parse));
    try(parse_simple(parse, &command->simple, &command->redirect));
    return true;
  case component_word:
    break;
  }
  return handle_multiple_segment(parse, command, component);
}

bool parse_command(struct parse *parse, struct command *command) {
  char c;
  parse_peek(parse, &c);
  assert(isblank(c) == false);

  if (c == '(') {
    try(command_select(command, command_subshell));
    try(parse_linebreak(parse));
    try(parse_subshell(parse, &command->subshell));
    return true;
  }

  // This is the best I can do. :(
  struct component component;
  component_init(&component);
  bool success = parse_command_component(parse, command, &component);
  component_destroy(&component);
  return success;
}
