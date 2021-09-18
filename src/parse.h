#ifndef PARSE_H
#define PARSE_H

#include <stdlib.h>
#include "config.h"
#include "log.h"

#define TOKEN_SIZE (LINE_SIZE / 2)

typedef struct {
  const char *file;
  char *const *argv;
  char *const *envp;
  const char *in;
  const char *out;
} command_t;

command_t command_init(char *line, char *token_list[TOKEN_SIZE]);
void command_debug(const command_t *cmd);

#endif // PARSE_H
