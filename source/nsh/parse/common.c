#include "nsh/parse/common.h"

bool parse_peek(struct parse *parse, char *ch) {
  assert(parse->track.size >= sizeof(struct track));
  struct track track;
  stack_back(&parse->track, &track, sizeof(struct track));
  char const *data = string_data(&track.value);
  *ch = data[track.index];
  return true;
}

bool parse_bump(struct parse *parse) {
  assert(parse->track.size >= sizeof(struct track));
  struct track *track = stack_tail(&parse->track, sizeof(struct track));
  char const *s = string_data(&track->value);
  if (parse->track.size == sizeof(struct track)) {
    if (s[track->index] == '\n') {
      parse->location.line++;
      parse->location.column = 0;
    } else if (s[track->index] == '\0') {
      assert(s[track->index] != '\0');
    } else {
      parse->location.column++;
    }
    track->index++;
  } else {
    track->index++;
    if (s[track->index] == '\0') {
      track_destroy(track);
      parse->track.size -= sizeof(struct track);
    }
  }
  return true;
}

bool parse_bump_peek(struct parse *restrict parse, char *ch) {
  try(parse_bump(parse));
  return parse_peek(parse, ch);
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
