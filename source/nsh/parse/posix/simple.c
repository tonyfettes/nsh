#include "nsh/parse/posix/simple.h"

#include "nsh/parse/posix/component.h"
#include "nsh/parse/posix/operator.h"
#include "nsh/parse/posix/delimiter.h"

static bool parse_simple_component(struct parse *parse,
                                   struct simple *simple,
                                   struct stack *redirect,
                                   struct component *component) {
  try(parse_component(parse, component, simple->argument.size == 0));
  switch (component->type) {
  case component_none:
    assert(component->type != component_none);
    break;
  case component_redirect:
    try(stack_push(redirect, &component->redirect,
                   sizeof(struct redirect)));
    break;
  case component_environment:
    try(stack_push(&simple->environment, &component->environment,
                   sizeof(struct environment)));
    break;
  case component_word:
    try(stack_push(&simple->argument, &component->word,
                   sizeof(struct word)));
    break;
  }
  component->type = component_none;
  return true;
}

bool parse_simple(struct parse *parse, struct simple *simple,
                  struct stack *redirect) {
  char c;
  parse_peek(parse, &c);
  assert(isspace(c) == false);
  bool ok = true;
  struct component component;
  component_init(&component);
  while (!is_control_operator(c)) {
    ok = parse_simple_component(parse, simple, redirect, &component);
    if (!ok) {
      break;
    }
    parse_blank(parse);
    parse_peek(parse, &c);
  }
  component_destroy(&component);
  return ok;
}
