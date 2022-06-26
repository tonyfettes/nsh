#include "nsh/parse/segment.h"

#include "common.h"

static bool run_segment(char const *input, struct segment *segment) {
  struct parse parse;
  parse_init(&parse);
  parse_setup(&parse, input);
  if (!parse_segment(&parse, segment)) {
    parse_destroy(&parse);
    return false;
  }
  parse_destroy(&parse);
  return true;
}

static void test_plain() {
  struct segment segment;
  segment_init(&segment);
  assert(run_segment("hello", &segment));
  assert(segment.type == segment_plain);
  assert(strcmp(string_data(&segment.plain), "hello") == 0);
  segment_destroy(&segment);
}

int main() {
  test_plain();
  return 0;
}
