#ifndef NSH_PARSE_COMMAND_H
#define NSH_PARSE_COMMAND_H

#include "nsh/command.h"
#include "nsh/parse/posix/common.h"

bool parse_command(struct parse *parse, struct command *command);

#endif // NSH_PARSE_COMMAND_H
