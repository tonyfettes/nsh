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

bool segment_expand(struct segment seg, struct string *target) {
  switch (seg.type) {
  case segment_none:
    assert(seg.type != segment_none);
    break;
  case segment_plain:
    if (!string_concat(target, seg.plain)) {
      return false;
    }
    break;
  case segment_parameter:
    if (!parameter_expand(seg.parameter, target)) {
      return false;
    }
    break;
  case segment_command:
    todo("Command substitution");
  }
  return true;
}
