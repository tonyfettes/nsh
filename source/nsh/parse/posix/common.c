#include "nsh/parse/posix/common.h"
#include "nsh/math.h"

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

void parse_peek_by(struct parse *parse, char *restrict target,
                   string_size_t *size) {
  assert(parse->track.size >= sizeof(struct track));
  size_t done = 0;
  struct track *iter = stack_tail(&parse->track, 0);
  struct track *head = stack_head(&parse->track, 0);
  while (done < *size && iter != head) {
    struct track *track = iter - 1;
    char const *data = string_data(&track->value);
    size_t copy_size = *size - done;
    if (track->value.size < copy_size) {
      copy_size = track->value.size;
    }
    memory_copy(target, data + track->index, copy_size);
    done += copy_size;
    // Decrement `iter`.
    iter = track;
  }
  *size = done;
}

void parse_peek(struct parse *parse, char *restrict ch) {
  string_size_t size = 1;
  parse_peek_by(parse, ch, &size);
  assert(size == 1);
}

bool parse_bump_by(struct parse *parse, string_size_t size) {
  assert(parse->track.size >= sizeof(struct track));
  struct track *iter = stack_tail(&parse->track, 0);
  while (size && iter != stack_head(&parse->track, 0)) {
    struct track *track = iter - 1;
    char const *s = string_data(&track->value);
    if (track == stack_head(&parse->track, 0)) {
      while (size) {
        if (s[track->index] == '\n') {
          parse->location.line++;
          parse->location.column = 0;
        } else {
          parse->location.column++;
        }
        track->index++;
        size--;
        if (s[track->index] == '\0') {
          todo("Reading input from stdin when parsing needs "
               "continuation");
        }
      }
    } else {
      while (size && s[track->index]) {
        track->index++;
        size--;
      }
      track_destroy(track);
      parse->track.size -= sizeof(struct track);
    }
  }
  return true;
}

bool parse_bump(struct parse *parse) {
  return parse_bump_by(parse, 1);
}

bool parse_bump_peek(struct parse *parse, char *ch) {
  try(parse_bump(parse));
  parse_peek(parse, ch);
  return true;
}

bool parse_read(struct parse *parse, char *restrict target,
                string_size_t *size) {
  parse_peek_by(parse, target, size);
  // We only bump up input stream by number of bytes read in. This
  // allows users to control whether to issue a 
  return parse_bump_by(parse, *size);
}

// bool parse_expect(struct parse *restrict parse,
//                   char const *restrict expected) {
//   char c;
//   if (!parse_peek(parse, &c)) {
//     return false;
//   }
//   for (; *expected != '\0'; expected++) {
//     if (c != *expected) {
//       try(stack_push(&parse->diagnosis, &(struct diagnosis) {
//         .level = diagnosis_error,
//         .type = diagnosis_unexpected,
//         .unexpect = {
//           .expected = expected,
//         },
//       }, sizeof(struct diagnosis)));
//       return false;
//     }
//   }
//   *sptr = cptr;
//   return true;
// }
//
// bool parse_diagnose(struct parse *parse, struct diagnosis diagnosis) {
//   return stack_push(&parse->diagnosis, &diagnosis, sizeof(diagnosis));
// }
