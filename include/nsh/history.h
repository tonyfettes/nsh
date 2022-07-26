#ifndef NSH_HISTORY_H
#define NSH_HISTORY_H

#include "nsh/stack.h"
#include "nsh/file.h"

struct history {
  struct stack line;
};

void history_load(struct history *history, struct file file);

void history_store(struct history *history, struct file file);

#endif // NSH_HISTORY_H
