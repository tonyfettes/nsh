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
  union {
    struct word target;
    unsigned int descriptor;
    struct string path;
    struct string heredoc;
  };
};

void redirect_init(struct redirect *redirect);

void redirect_clear(struct redirect *redirect);

void redirect_destroy(struct redirect *redirect);

#endif // NSH_REDIRECT_H
