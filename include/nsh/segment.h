#ifndef NSH_SEGMENT_H
#define NSH_SEGMENT_H

#include "nsh/string.h"
#include "nsh/parameter.h"
#include "nsh/command.h"
#include "nsh/quote.h"

struct segment {
  enum segment_type {
    segment_plain,
    segment_parameter,
    segment_command,
  } type;
  enum quote quote;
  union {
    struct string plain;
    struct parameter parameter;
    struct command command;
  };
};

void segment_init(struct segment *segment);

void segment_select(struct segment *segment, enum segment_type type);

void segment_destroy(struct segment *segment);

bool segment_expand(struct segment segment, struct string *target);

#endif // NSH_SEGMENT_H
