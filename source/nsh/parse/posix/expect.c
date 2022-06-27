#include "nsh/parse/common.h"

/*
extern bool parse_combine(parse_t parse[], size_t size,
                          struct context *restrict context,
                          void *data) {
  struct context error;
  context_init(&error);
  struct snapshot snapshot;
  context_save(context, &snapshot);
  for (size_t i = 0; i < size; i++) {
    if (parse[i](context, data)) {
      return true;
    }
    context_merge(context, &snapshot, &error);
    context_restore(context, &snapshot);
  }
  context_update(context, &error);
  context_destroy(&error);
  return false;
}
*/

// extern bool parse_peek_char(char const **restrict sptr,
//                             char *restrict ch) {
//   char const *cptr = *sptr;
//   while (*cptr == '\\' && *(cptr + 1) == '\n') {
//     if (*(cptr + 2) == '\0') {
//     } else {
//       cptr += 2;
//     }
//   }
//   *sptr = cptr;
//   *ch = *cptr;
//   return true;
// }

// extern bool parse_raw(char **restrict sptr, char *restrict out,
//                       size_t size, struct stack *restrict diagnosis) {
//   char *cptr = *sptr;
//   char c;
//   for (size_t i = 0; (c = peek(cptr)) && i < size; i++, cptr++) {
//     out[i] = c;
//   }
//   return true;
// }

