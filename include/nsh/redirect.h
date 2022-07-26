#ifndef NSH_REDIRECT_H
#define NSH_REDIRECT_H

#include "nsh/string.h"
#include "nsh/word.h"

struct redirect {
  // Source file descriptor.
  int source;
  enum redirect_type {
    // `<&` or `>&`
    redirect_descriptor,
    // `<`, `<>`, `>`, `>|`, `>>`
    redirect_path,
    // `<<`, `<<-` (O_TRUNC)
    redirect_heredoc,
  } type;
  int flags;
  struct word target;
};

void redirect_init(struct redirect *redirect);

void redirect_clear(struct redirect *redirect);

void redirect_destroy(struct redirect *redirect);

bool redirect_display(struct redirect const *redirect,
                      struct string *target);

#endif // NSH_REDIRECT_H
