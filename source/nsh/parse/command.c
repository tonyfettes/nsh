#include "nsh/parse/command.h"

#include "nsh/context.h"
#include "nsh/parse/delimiter.h"
#include "nsh/parse/block.h"
#include "nsh/parse/component.h"
#include "nsh/parse/simple.h"
#include "nsh/parse/segment.h"
#include "nsh/parse/loop.h"
#include "nsh/parse/function.h"
#include "nsh/parse/compound.h"

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

  if (component->word.quoted) {
    goto cont_func_simp;
  }

  assert(component->word.segment.size == sizeof(struct segment));
  struct segment *segment = component->word.segment.data;
  assert(segment->type == segment_plain);

  enum keyword keyword = keyword_from(segment->plain);
  switch (keyword) {
  case keyword_brace_open:
    try(command_select(command, command_brace));
    try(parse_linebreak(parse));
    try(parse_brace(parse, &command->brace));
    try(parse_blank(parse));
    try(parse_redirect_list(parse, &command->redirect));
    return true;
  case keyword_while:
    try(command_select(command, command_while_loop));
    try(parse_linebreak(parse));
    try(parse_while_loop(parse, &command->while_loop));
    try(parse_blank(parse));
    try(parse_redirect_list(parse, &command->redirect));
    return true;
  case keyword_until:
    try(command_select(command, command_until_loop));
    try(parse_linebreak(parse));
    try(parse_until_loop(parse, &command->until_loop));
    try(parse_blank(parse));
    try(parse_redirect_list(parse, &command->redirect));
    return true;
  case keyword_for:
    try(command_select(command, command_for_loop));
    try(parse_linebreak(parse));
    try(parse_for_loop(parse, &command->for_loop));
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

  struct track *track = parse->track.data;
  size_t track_size = parse->track.size / sizeof(struct track);
  for (size_t i = 0; i < track_size; i++) {
    if (string_equal(segment->plain, track->name)) {
      goto cont_func_simp;
    }
  }

  struct string *alias_value;
  if (alias_table_find(&context.alias_table, segment->plain,
                       &alias_value)) {
    try(stack_alloc(&parse->track, sizeof(struct track)));
    struct track *track = stack_tail(&parse->track, 0);
    track_init(track);
    // Move `segment->plain` into `track.name`.
    string_swap(&track->name, &segment->plain);
    word_destroy(&component->word);
    component->type = component_none;
    // Copy `*alias_value` into `track.value`.
    if (!string_append(&track->value, *alias_value)) {
      track_destroy(track);
      return false;
    }
    assert(stack_bump(&parse->track, sizeof(struct track)));
    // TODO: call parse_complete() here to re-parse.
    try(parse_blank(parse));
    try(parse_command(parse, command));
    return true;
  }

  if (catch(hash, not_exist)) {
    goto cont_func_simp;
  }

  return false;

cont_func_simp:
  try(parse_blank(parse));
  char c;
  try(parse_peek(parse, &c));
  if (c == '(') {
    try(parse_blank(parse));
    try(parse_peek(parse, &c));
    if (c != ')') {
      try(stack_push(&parse->diagnosis, &(struct diagnosis) {
        .level = diagnosis_error,
        .type = diagnosis_unexpected,
        .location = parse->location,
        .unexpect = {
          .expected = ")",
        },
      }, sizeof(struct diagnosis)));
      throw(parse, invalid_syntax);
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
                   sizeof(component->word)));
    component->type = component_none;
    try(parse_simple(parse, &command->simple, &command->redirect));
  }
  return true;
}

bool parse_command(struct parse *parse, struct command *command) {
  char c;
  try(parse_peek(parse, &c));
  assert(isblank(c) == false);

  if (c == '(') {
    try(command_select(command, command_subshell));
    try(parse_linebreak(parse));
    try(parse_subshell(parse, &command->subshell));
    try(parse_peek(parse, &c));
    if (c != ')') {
      try(stack_push(&parse->diagnosis, &(struct diagnosis) {
        .level = diagnosis_error,
        .type = diagnosis_unexpected,
        .unexpect = {
          .expected = ")",
        },
      }, sizeof(struct diagnosis)));
      return false;
    }
    return true;
  }

  // This is the best I can do. :(
  struct component component;
  component_init(&component);
  bool success = parse_command_component(parse, command, &component);
  component_destroy(&component);
  return success;
}
