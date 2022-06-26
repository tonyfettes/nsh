#ifndef NSH_COMPOUND_H
#define NSH_COMPOUND_H

#include "nsh/block.h"

struct subshell {
  struct block body;
};

void subshell_init(struct subshell *subshell);

void subshell_clear(struct subshell *subshell);

void subshell_destroy(struct subshell *subshell);

struct brace {
  struct block body;
};

void brace_init(struct brace *brace);

void brace_clear(struct brace *brace);

void brace_destroy(struct brace *brace);

#endif // NSH_COMPOUND_H
