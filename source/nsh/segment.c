#include "nsh/segment.h"

void segment_init(struct segment *segment) {
  segment->type = segment_none;
}

void segment_select(struct segment *segment, enum segment_type type) {
  segment_destroy(segment);
  segment->type = type;
  switch (type) {
  case segment_none:
    break;
  case segment_plain:
    string_init(&segment->plain);
    break;
  case segment_parameter:
    parameter_init(&segment->parameter);
    break;
  case segment_command:
    command_init(&segment->command);
    break;
  }
}

void segment_destroy(struct segment *segment) {
  switch (segment->type) {
  case segment_none:
    break;
  case segment_plain:
    string_destroy(&segment->plain);
    break;
  case segment_parameter:
    parameter_destroy(&segment->parameter);
    break;
  case segment_command:
    command_destroy(&segment->command);
    break;
  }
  segment->type = segment_none;
}

bool segment_expand(struct segment *segment) {
  switch (segment->type) {
  case segment_none:
  case segment_plain:
    break;
  case segment_parameter:
    // string_init(&result);
    // ok = parameter_expand(&segment->parameter, &result);
    // string_destroy(&result);
    // return ok;
    todo("Parameter expansion");
    break;
  case segment_command:
    todo("Command substitution");
    break;
  }
  return false;
}
