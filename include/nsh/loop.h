#ifndef NSH_LOOP_H
#define NSH_LOOP_H

#include "nsh/block.h"
#include "nsh/string.h"
#include "nsh/stack.h"

struct loop {
  enum {
    loop_while,
    loop_until,
  } type;
  struct block condition;
  struct block body;
};

void loop_init(struct loop *loop);

void loop_clear(struct loop *loop);

void loop_destroy(struct loop *loop);

struct for_in {
  struct string name;
  struct stack word_list;
  struct block body;
};

void for_in_init(struct for_in *for_in);

void for_in_clear(struct for_in *for_in);

bool for_in_execute(struct for_in for_in, struct stack *job);

void for_in_destroy(struct for_in *for_in);

#endif // NSH_LOOP_H
