#ifndef NSH_PROCESS_H
#define NSH_PROCESS_H

#include "nsh/common.h"
#include "nsh/stack.h"
#include "nsh/simple.h"
#include "nsh/file.h"

struct process {
  pid_t pid;
};

struct program {
  struct string path;
  // Stack of `struct string`, expanded.
  struct stack argument;
  // Stack of `struct environment`.
  struct stack environment;
};

void program_init(struct program *program);

void program_clear(struct program *program);

void program_destroy(struct program *program);

struct group {
  bool foreground;
  struct process leader;
  // Stack of `struct process`.
  struct stack member;
};

void group_init(struct group *group);

bool group_spawn(struct group *group, struct string path,
                 struct simple simple, struct stack redirect,
                 struct pipe *pipe);

bool group_fork(struct group *group, bool (*func)(void *data),
                void *data, struct stack redirect, struct pipe *pipe);

void group_destroy(struct group *group);

bool process_wait(struct process *process, int *status, int flags);

#endif // NSH_PROCESS_H
