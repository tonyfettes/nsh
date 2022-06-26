#include "nsh/compound.h"

void subshell_init(struct subshell *subshell) {
  block_init(&subshell->body);
}

void subshell_clear(struct subshell *subshell) {
  block_clear(&subshell->body);
}

void subshell_destroy(struct subshell *subshell) {
  block_destroy(&subshell->body);
}

void brace_init(struct brace *brace) {
  block_init(&brace->body);
}

void brace_clear(struct brace *brace) {
  block_clear(&brace->body);
}

void brace_destroy(struct brace *brace) {
  block_destroy(&brace->body);
}
