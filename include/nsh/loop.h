#ifndef NSH_LOOP_H
#define NSH_LOOP_H

#include "nsh/block.h"
#include "nsh/string.h"
#include "nsh/stack.h"

struct while_loop {
  struct block condition;
  struct block body;
};

void while_loop_init(struct while_loop *while_loop);

void while_loop_clear(struct while_loop *while_loop);

void while_loop_destroy(struct while_loop *while_loop);

struct until_loop {
  struct block condition;
  struct block body;
};

void until_loop_init(struct until_loop *until_loop);

void until_loop_clear(struct until_loop *until_loop);

void until_loop_destroy(struct until_loop *until_loop);

struct for_loop {
  struct string name;
  struct stack word_list;
  struct block body;
};

void for_loop_init(struct for_loop *for_loop);

void for_loop_clear(struct for_loop *for_loop);

void for_loop_destroy(struct for_loop *for_loop);

#endif // NSH_LOOP_H
