#include "nsh/parse/posix/component.h"

#include "nsh/parse/posix/delimiter.h"
#include "nsh/parse/posix/name.h"
#include "nsh/parse/posix/redirect.h"

void component_init(struct component *component) {
  component->type = component_none;
}

void component_select(struct component *component,
                      enum component_type type) {
  if (component->type == type) {
    switch (type) {
    case component_none:
      break;
    case component_redirect:
      redirect_clear(&component->redirect);
      break;
    case component_environment:
      environment_clear(&component->environment);
      break;
    case component_word:
      word_clear(&component->word);
      break;
    }
  } else {
    component_destroy(component);
    component->type = type;
    switch (type) {
    case component_none:
      break;
    case component_redirect:
      redirect_init(&component->redirect);
      break;
    case component_environment:
      environment_init(&component->environment);
      break;
    case component_word:
      word_init(&component->word);
      break;
    }
  }
}

void component_destroy(struct component *component) {
  switch (component->type) {
  case component_none:
    break;
  case component_redirect:
    redirect_destroy(&component->redirect);
    break;
  case component_environment:
    environment_destroy(&component->environment);
    break;
  case component_word:
    word_destroy(&component->word);
    break;
  }
}

struct parse_component_environment_param {
  struct parse *parse;
  struct component *component;
  struct string *name;
};

static inline bool handle_environment(struct parse *parse,
                                      struct component *component,
                                      struct string *name) {
  try(parse_name(parse, name));
  char c;
  try(parse_peek(parse, &c));
  if (c == '=') {
    component_select(component, component_environment);
    string_swap(&component->environment.name, name);
    try(parse_bump(parse));
    try(parse_word(parse, &component->environment.value));
  } else {
    component_select(component, component_word);
    try(word_puts(&component->word, name));
    if (!is_delimiter(parse, c)) {
      try(parse_word(parse, &component->word));
    }
  }
  return true;
}

bool parse_component(struct parse *parse, struct component *component,
                     bool environment) {
  char c;
  parse_peek(parse, &c);
  assert(c == '<' || c == '>' || !is_delimiter(parse, c));

  if (isdigit(c)) {
    struct string string;
    string_init(&string);
    int source;
    try(parse_redirect_source(parse, &source, &string));
    if (source != -1) {
      string_destroy(&string);
      component_select(component, component_redirect);
      component->redirect.source = source;
      try(parse_redirect(parse, &component->redirect));
    } else {
      component_select(component, component_word);
      try(word_puts(&component->word, &string));
      parse_peek(parse, &c);
      if (!is_delimiter(parse, c)) {
        try(parse_word(parse, &component->word));
      }
    }
    return true;
  }

  if (c == '<' || c == '>') {
    component_select(component, component_redirect);
    component->redirect.source = -1;
    try(parse_redirect(parse, &component->redirect));
    return true;
  }

  if (environment && (c == '_' || isalpha(c))) {
    struct string name;
    string_init(&name);
    bool ok = handle_environment(parse, component, &name);
    string_destroy(&name);
    return ok;
  }

  component_select(component, component_word);
  try(parse_word(parse, &component->word));
  return true;
}
