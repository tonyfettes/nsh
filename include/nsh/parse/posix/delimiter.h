#ifndef NSH_PARSE_DELIMITER_H
#define NSH_PARSE_DELIMITER_H

#include "nsh/parse/posix/common.h"

bool is_delimiter(struct parse *parse, char ch);

bool parse_delimiter(struct parse *parse);

bool parse_blank(struct parse *parse);

bool parse_linebreak(struct parse *parse);

#endif // NSH_PARSE_DELIMITER_H
