#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "config.h"
#include "log.h"
#include "parse.h"

int main(int argc, char *argv[]) {
  // read eval print loop
  char line[LINE_SIZE];
  line[LINE_SIZE - 2] = '\n';
  char *token_list[TOKEN_SIZE];
  while (fgets(line, LINE_SIZE, stdin) != NULL) {
    command_t cmd = command_init(line, token_list);
    command_debug(&cmd);
    pid_t pid = fork();
    if (pid == 0) {
      pid = getpid();
      setpgid(pid, pid);
      tcsetpgrp(STDIN_FILENO, pid);
      if (execvp(cmd.file, cmd.argv) == -1) {
        error("unable to execute command");
      }
    } else {
      setpgid(pid, pid);
      tcsetpgrp(STDIN_FILENO, pid);
      while (waitpid(-1, NULL, 0) < 0);
    }
  }
  return 0;
}
