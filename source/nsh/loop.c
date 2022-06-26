#include "nsh/loop.h"

void while_loop_init(struct while_loop *while_loop) {
  block_init(&while_loop->condition);
  block_init(&while_loop->body);
}

void while_loop_clear(struct while_loop *while_loop) {
  block_clear(&while_loop->condition);
  block_clear(&while_loop->body);
}

void while_loop_destroy(struct while_loop *while_loop) {
  block_destroy(&while_loop->condition);
  block_destroy(&while_loop->body);
}

void until_loop_init(struct until_loop *until_loop) {
  block_init(&until_loop->condition);
  block_init(&until_loop->body);
}

void until_loop_clear(struct until_loop *until_loop) {
  block_clear(&until_loop->condition);
  block_clear(&until_loop->body);
}

void until_loop_destroy(struct until_loop *until_loop) {
  block_destroy(&until_loop->condition);
  block_destroy(&until_loop->body);
}

void for_loop_init(struct for_loop *for_loop) {
  string_init(&for_loop->name);
  stack_init(&for_loop->word_list);
  block_init(&for_loop->body);
}

void for_loop_clear(struct for_loop *for_loop) {
  string_clear(&for_loop->name);
  stack_clear(&for_loop->word_list);
  block_clear(&for_loop->body);
}

void for_loop_destroy(struct for_loop *for_loop) {
  string_destroy(&for_loop->name);
  stack_destroy(&for_loop->word_list);
  block_destroy(&for_loop->body);
}
