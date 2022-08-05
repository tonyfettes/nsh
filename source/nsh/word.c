#include "nsh/word.h"
#include "nsh/segment.h"
#include "nsh/user.h"
#include "nsh/file.h"
#include <sys/mman.h>

void word_init(struct word *word) {
  word->quoted = false;
  stack_init(&word->segment);
}

void word_clear(struct word *word) {
  stack_clear(&word->segment);
}

void word_destroy(struct word *word) {
  for (struct segment *segment = stack_head(&word->segment, 0);
      segment != stack_tail(&word->segment, 0); segment++) {
    segment_destroy(segment);
  }
  stack_destroy(&word->segment);
  word_init(word);
}

bool word_push(struct word *word, struct segment *segment) {
  try(stack_push(&word->segment, segment, sizeof(*segment)));
  segment_init(segment);
  return true;
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
    try(string_concat(&segment->plain, *string));
    string_clear(string);
  }
  return true;
}

static bool expand_command(struct command command,
                           struct string *target) {
  // (void) command;
  // (void) target;
  // todo("Command substitution");
  // struct pipe pipe;
  // pipe_open(&pipe);
  // execute_command(NULL, command, pipe);
  int mfd = memfd_create("nsh_popen", MFD_CLOEXEC);
  if (mfd == -1) {
    return false;
  }
  struct pipe pipe;
  pipe.write = mfd;
  pipe.read = -1;
  execute_command(NULL, command, pipe);
}

static bool expand_tilde(struct word word, struct string *target) {
  bool ok = true;

  struct segment *seg = stack_head(&word.segment, 0);

  switch (seg->type) {
  case segment_none:
    assert(seg->type != segment_none);
  case segment_parameter:
    return parameter_expand(seg->parameter, target);
  case segment_command:
    return expand_command(seg->command, target);
  case segment_plain:
    break;
  }

  assert(seg->plain.size > 0);
  if (seg->quote) {
    return true;
  }

  char const *plain = string_data(&seg->plain);
  if (plain[0] != '~') {
    return string_concat(target, seg->plain);
  }

  string_size_t slash_index;
  struct string user;
  string_init(&user);
  if (string_find(seg->plain, '/', &slash_index)) {
    ok = string_append(&user, seg->plain, 1, slash_index);
    if (!ok) goto exit;
  } else if (word.segment.size == sizeof(struct segment)) {
    ok = string_append(&user, seg->plain, 1, seg->plain.size);
    if (!ok) goto exit;
  } else {
    return true;
  }

  ok = user_home(user, target);

exit:
  string_destroy(&user);
  return ok;
}

bool word_expand(struct word word, struct string *target) {
  if (word.segment.size == 0) {
    return true;
  }
  if (!expand_tilde(word, target)) {
    return false;
  }
  struct segment *head = stack_head(&word.segment, 0);
  struct segment *tail = stack_tail(&word.segment, 0);
  for (struct segment *seg = head; seg != tail; seg++) {
    if (!segment_expand(*seg, target)) {
      return false;
    }
  }
  return true;
}

bool word_split(struct word word, struct stack *target,
                struct string delimiter) {
  struct string expansion;
  string_init(&expansion);
  if (!word_expand(word, &expansion)) {
    string_destroy(&expansion);
    return false;
  }
  if (!string_split(expansion, target, delimiter)) {
    string_destroy(&expansion);
    return false;
  }
}
