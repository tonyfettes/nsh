#include "nsh/loop.h"

void loop_init(struct loop *loop) {
  block_init(&loop->condition);
  block_init(&loop->body);
}

void loop_clear(struct loop *loop) {
  block_clear(&loop->condition);
  block_clear(&loop->body);
}

void loop_destroy(struct loop *loop) {
  block_destroy(&loop->condition);
  block_destroy(&loop->body);
}

void for_in_init(struct for_in *for_in) {
  string_init(&for_in->name);
  stack_init(&for_in->word_list);
  block_init(&for_in->body);
}

void for_in_clear(struct for_in *for_in) {
  string_clear(&for_in->name);
  stack_clear(&for_in->word_list);
  block_clear(&for_in->body);
}

void for_in_destroy(struct for_in *for_in) {
  string_destroy(&for_in->name);
  stack_destroy(&for_in->word_list);
  block_destroy(&for_in->body);
}
