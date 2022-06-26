#ifndef NSH_PARAMETER_H
#define NSH_PARAMETER_H

#include "nsh/common.h"
#include "nsh/string.h"
#include "nsh/word.h"

struct parameter {
  enum parameter_type {
    // ${name}
    parameter_simple,
    // ${name[:](-|=|?|+)word}
    parameter_default = '-',
    parameter_assign = '=',
    parameter_indicate_null = '?',
    parameter_alternative = '+',
    // ${#name}
    parameter_length,
    // ${name(%|#)[(%|#)]pattern}
    parameter_suffix = '%',
    parameter_prefix = '#',
  } type;
  union {
    bool substitute_null;
    bool longest_match;
  };
  struct string name;
  struct word word;
};

void parameter_init(struct parameter *parameter);

bool is_special_parameter(char ch);

bool parameter_expand(struct parameter *parameter, struct string *out);

void parameter_destroy(struct parameter *parameter);

#endif // NSH_PARAMETER_H
