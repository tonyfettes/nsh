#ifndef NSH_EXECUTE_REDIRECT_H
#define NSH_EXECUTE_REDIRECT_H

#include "nsh/redirect.h"
#include <spawn.h>

void execute_redirect_fork(struct redirect *redirect);

void execute_redirect_fact(struct redirect *redirect,
                           posix_spawn_file_actions_t *fact);

#endif // NSH_EXECUTE_REDIRECT_H
