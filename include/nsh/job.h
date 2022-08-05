#ifndef NSH_JOB_H
#define NSH_JOB_H

#include "nsh/process.h"
#include "nsh/hash.h"

struct job {
  unsigned int id;
  struct group group;
  struct string name;
  // Stack of `int`. To be closed when destructed.
  struct stack file_descriptor;
};

struct job_option {
  bool control;
  enum job_option_notify {
    job_option_notify_none,
    job_option_notify_sync,
    job_option_notify_async,
  } notify;
};

bool job_init(struct job *job);

void job_destroy(struct job *job);

bool job_display(struct job *job, struct string *target);

bool job_is_empty(struct job job);

#endif // NSH_JOB_H
