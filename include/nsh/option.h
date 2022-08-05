#ifndef NSH_OPTION_H
#define NSH_OPTION_H

#include "nsh/job.h"

struct option {
  struct job_option job;
};

void option_init(struct option *option);

#endif // NSH_OPTION_H
