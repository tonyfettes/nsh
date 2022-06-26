#include "nsh/parse.h"
#include "nsh/parse/common.h"

int error_parse = -1;

void track_init(struct track *track) {
  string_init(&track->name);
  string_init(&track->value);
  track->index = 0;
}

void track_destroy(struct track *track) {
  string_destroy(&track->name);
  string_destroy(&track->value);
}

void parse_init(struct parse *parse) {
  stack_init(&parse->track);
  parse->parameter_level = 0;
  stack_init(&parse->diagnosis);
}

void parse_destroy(struct parse *parse) {
  struct track *track = parse->track.data;
  for (size_t i = 0; i < parse->track.size / sizeof(struct track);
       i++) {
    track_destroy(&track[i]);
  }
  stack_destroy(&parse->track);
  stack_destroy(&parse->diagnosis);
}
