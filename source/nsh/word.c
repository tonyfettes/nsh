#include "nsh/word.h"
#include "nsh/segment.h"

void word_init(struct word *word) {
  word->quoted = false;
  word->type = word_segment;
  stack_init(&word->segment);
}

static void word_destroy_segment(struct word *word) {
  struct segment *segment = word->segment.data;
  size_t size = word->segment.size / sizeof(struct segment);
  for (size_t i = 0; i < size; i++) {
    segment_destroy(&segment[i]);
  }
}

static void word_destroy_field(struct word *word) {
  struct string *field = word->field.data;
  size_t size = word->field.size / sizeof(struct string);
  for (size_t i = 0; i < size; i++) {
    string_destroy(&field[i]);
  }
}

void word_select(struct word *word, enum word_type type) {
  (void) word, (void) type;
  todo("Switching enumerator in word");
}

void word_clear(struct word *word) {
  word_destroy(word);
}

void word_destroy(struct word *word) {
  switch (word->type) {
  case word_segment:
    word_destroy_segment(word);
    stack_destroy(&word->segment);
    break;
  case word_plain:
    string_destroy(&word->plain);
    break;
  case word_field:
    word_destroy_field(word);
    stack_destroy(&word->field);
    break;
  }
  word_init(word);
}

bool word_putc(struct word *word, char ch) {
  struct segment *segment;
  segment = stack_tail(&word->segment, sizeof(struct segment));
  if (word->segment.size == 0 || segment->type != segment_plain) {
    try(stack_alloc(&word->segment, sizeof(struct segment)));
    struct segment *segment = stack_tail(&word->segment, 0);
    segment_init(segment);
    segment_select(segment, segment_plain);
    if (!string_putc(&segment->plain, ch)) {
      segment_destroy(segment);
      return false;
    }
    assert(stack_bump(&word->segment, sizeof(struct segment)));
  } else {
    try(string_putc(&segment->plain, ch));
  }
  return true;
}

bool word_puts(struct word *word, struct string *string) {
  struct segment *segment;
  segment = stack_tail(&word->segment, sizeof(struct segment));
  if (word->segment.size == 0 || segment->type == segment_plain) {
    try(stack_alloc(&word->segment, sizeof(struct segment)));
    struct segment *segment = stack_tail(&word->segment, 0);
    segment_init(segment);
    segment_select(segment, segment_plain);
    string_swap(&segment->plain, string);
    assert(stack_bump(&word->segment, sizeof(struct segment)));
  } else {
    try(string_append(&segment->plain, *string));
    string_clear(string);
  }
  return true;
}

bool word_expand(struct word *word) {
  (void) word;
  todo("Word expansion");
}
