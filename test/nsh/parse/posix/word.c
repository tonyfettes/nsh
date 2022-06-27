#include "nsh/parse/posix/word.h"
#include "nsh/parse/posix/segment.h"

#include "common.h"

static bool run_word(char const *input, struct word *word) {
  struct parse parse;
  parse_init(&parse);
  parse_setup(&parse, input);
  bool ok = parse_word(&parse, word);
  parse_destroy(&parse);
  return ok;
}

static void test_plain() {
  struct word word;
  word_init(&word);
  assert(run_word("hello", &word));
  assert(word.segment.size == sizeof(struct segment));
  assert(word.quoted == false);
  struct segment *segment = word.segment.data;
  assert(segment->type == segment_plain);
  assert(strcmp(string_data(&segment->plain), "hello") == 0);
  word_destroy(&word);
}

static void test_escape_alnum() {
  struct word word;
  word_init(&word);
  assert(run_word("he\\ll\\o", &word));
  assert(word.segment.size == sizeof(struct segment));
  assert(word.quoted == true);
  struct segment *segment = word.segment.data;
  assert(segment->type == segment_plain);
  assert(strcmp(string_data(&segment->plain), "hello") == 0);
  word_destroy(&word);
}

static void test_escape_linebreak() {
  struct word word;
  word_init(&word);
  assert(run_word("he\\\nllo", &word));
  assert(word.segment.size == sizeof(struct segment));
  assert(word.quoted == false);
  struct segment *segment = word.segment.data;
  assert(segment->type == segment_plain);
  assert(strcmp(string_data(&segment->plain), "hello") == 0);
  word_destroy(&word);
}

static void test_escape() {
  test_escape_alnum();
  test_escape_linebreak();
}

static void test_single_quoted() {
  struct word word;
  word_init(&word);
  assert(run_word("he'${ll}'o", &word));
  assert(word.segment.size == sizeof(struct segment));
  assert(word.quoted == true);
  struct segment *segment = word.segment.data;
  assert(segment->type == segment_plain);
  assert(strcmp(string_data(&segment->plain), "he${ll}o") == 0);
  word_destroy(&word);
}

static void test_double_quoted() {
  struct word word;
  word_init(&word);
  assert(run_word("he\"\\${ll}\"o", &word));
  assert(word.segment.size == sizeof(struct segment));
  assert(word.quoted == true);
  struct segment *segment = word.segment.data;
  assert(segment->type == segment_plain);
  assert(strcmp(string_data(&segment->plain), "he${ll}o") == 0);
  word_destroy(&word);
}

static void test_parameter() {
  struct word word;
  word_init(&word);
  assert(run_word("he${ll}o", &word));
  assert(word.segment.size == 3 * sizeof(struct segment));
  assert(word.quoted == true);
  struct segment *segment = word.segment.data;

  assert(segment[0].type == segment_plain);
  assert(strcmp(string_data(&segment[0].plain), "he") == 0);

  assert(segment[1].type == segment_parameter);
  assert(segment[1].parameter.type == parameter_simple);
  assert(strcmp(string_data(&segment[1].parameter.name), "ll") == 0);
  
  assert(segment[2].type == segment_plain);
  assert(strcmp(string_data(&segment[2].plain), "o") == 0);

  word_destroy(&word);
}

int main() {
  test_plain();
  test_escape();
  test_single_quoted();
  test_double_quoted();
  test_parameter();
  return 0;
}
