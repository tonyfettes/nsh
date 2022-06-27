#ifndef TEST_NSH_PARSE_COMMON_H
#define TEST_NSH_PARSE_COMMON_H

#include "nsh/parse/posix/common.h"

#include <stdio.h>

void parse_setup(struct parse *parse, char const *line);

#define parse(retval, type, input, ...)                                \
  do {                                                                 \
    struct parse parse;                                                \
    parse_init(&parse);                                                \
    parse_setup(&parse, input);                                        \
    retval = parse_##type(&parse, __VA_ARGS__);                        \
    parse_destroy(&parse);                                             \
  } while (0);

#endif // TEST_NSH_PARSE_COMMON_H
