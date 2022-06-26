#include "nsh/redirect.h"

static void redirect_init_source_flags(struct redirect *redirect) {
  redirect->source = -1;
  redirect->flags = 0;
}

void redirect_init(struct redirect *redirect) {
  redirect_init_source_flags(redirect);
  word_init(&redirect->target);
}

void redirect_clear(struct redirect *redirect) {
  redirect_init_source_flags(redirect);
  word_clear(&redirect->target);
}

void redirect_destroy(struct redirect *redirect) {
  redirect_init_source_flags(redirect);
  word_destroy(&redirect->target);
}
