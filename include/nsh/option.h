#ifndef NSH_OPTION_H
#define NSH_OPTION_H

#include "nsh/job.h"

enum option_fork_condition {
  option_fork_async = 1 << 0,
  option_fork_substituion = 1 << 1,
  option_fork_pipeline_last = 1 << 2,
  option_fork_pipeline_rest = 1 << 3,
  option_fork_pipeline_all = option_fork_pipeline_rest |
                             option_fork_pipeline_last,
  option_fork_posix = option_fork_async |
                      option_fork_substituion |
                      option_fork_pipeline_all,
  option_fork_zsh = option_fork_async |
                    option_fork_substituion |
                    option_fork_pipeline_rest,
};

struct option {
  struct job_option job;
  unsigned int fork_condition;
};

#endif // NSH_OPTION_H
