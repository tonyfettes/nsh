#include "common.h"
#include "nsh/parse/simple.h"

void parse_setup(struct parse *parse, char const *line) {
  assert(stack_alloc(&parse->track, sizeof(struct track)));
  struct track *track = parse->track.data;
  track_init(track);
  assert(string_puts(&track->value, line));
  assert(stack_bump(&parse->track, sizeof(struct track)));
}
