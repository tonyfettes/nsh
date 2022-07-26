#ifndef NSH_JOB_H
#define NSH_JOB_H

#include "nsh/statement.h"
#include "nsh/process.h"
#include "nsh/hash.h"

struct job {
  unsigned int id;
  struct group group;
  struct string name;
};

struct job_option {
  bool job_control;
  bool job_async_notification;
};

bool job_init(struct job *job);

void job_destroy(struct job *job);

bool job_execute(struct job *job, struct pipeline pipeline);

bool job_display(struct job *job, struct string *target);

#endif // NSH_JOB_H
