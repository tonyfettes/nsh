#include "parse.h"

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define skip(...) { \
  while (__VA_ARGS__) { \
    ptr++; \
  } \
}

int isquote(int ch) {
  return ch == '\'';
}

command_t command_init(char *line, char *token_list[TOKEN_SIZE]) {
  char *token = line;
  char *ptr = token;
  size_t token_list_length = 0;
  // bool single_quoted = false;
  // bool double_quoted = false;
  bool envp_ended = false;
  bool is_env = false;
  bool end = false;
  command_t command;
  command.envp = token_list;
  while (!end) {
    switch (*ptr) {
    case '\'':
      ptr++;
      token++;
      skip(*ptr != '\'');
      *ptr = '\0';
      ptr++;
      break;
    case '=':
      ptr++;
      if (envp_ended == false) {
        is_env = true;
      }
      break;
    case '<':
      ptr++;
      skip(isspace(*ptr));
      command.in = ptr;
      skip(isgraph(*ptr));
      if (isspace(*ptr)) {
        *ptr = '\0';
        skip(isspace(*ptr));
        token = ptr;
      } else if (*ptr == '\0') {
        end = true;
      }
      break;
    case '>':
      ptr++;
      skip(isspace(*ptr));
      command.out = ptr;
      skip(isgraph(*ptr));
      if (isspace(*ptr)) {
        *ptr = '\0';
        skip(isspace(*ptr));
        token = ptr;
      } else if (*ptr == '\0') {
        end = true;
      }
      break;
    case ' ':
    case '\t':
    case '\n':
      if (envp_ended == false && is_env == false) {
        token_list[token_list_length++] = NULL;
        command.file = token;
        command.argv = token_list + token_list_length;
        envp_ended = true;
      }
      *ptr = '\0';
      ptr++;
      token_list[token_list_length++] = token;
      skip(isspace(*ptr));
      token = ptr;
      break;
    case '\0':
      if (token != ptr) {
        token_list[token_list_length++] = token;
      }
      end = true;
      break;
    default:
      ptr++;
      break;
    }
  }
  token_list[token_list_length++] = NULL;
  return command;
}

void command_debug(const command_t *cmd) {
  debug("command_t {");
  debug("  file: \"%s\",", cmd->file);
  debug("  argv: [");
  for (int i = 0; cmd->argv[i] != NULL; i++) {
    debug("    \"%s\",", cmd->argv[i]);
  }
  debug("  ],");
  debug("  in: \"%s\",", cmd->in);
  debug("  out: \"%s\",", cmd->out);
  debug("}");
}
