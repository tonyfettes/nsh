#ifndef NSH_PARSE_COMPONENT_H
#define NSH_PARSE_COMPONENT_H

#include "nsh/parse/redirect.h"
#include "nsh/parse/environment.h"
#include "nsh/parse/word.h"

struct component {
  enum component_type {
    component_none,
    component_redirect,
    component_environment,
    component_word,
  } type;
  union {
    struct redirect redirect;
    struct environment environment;
    struct word word;
  };
};

void component_init(struct component *component);

void component_select(struct component *component,
                      enum component_type type);

void component_destroy(struct component *component);

bool parse_component(struct parse *parse, struct component *component, bool environment);

#endif // NSH_PARSE_COMPONENT_H
