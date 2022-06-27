#ifndef NSH_PARSE_STATEMENT_H
#define NSH_PARSE_STATEMENT_H

#include "nsh/parse/posix/common.h"
#include "nsh/statement.h"

// Parse statement, and consume following `&`, `;`, or `&&`
bool parse_statement(struct parse *parse, struct statement *statement);

#endif // NSH_PARSE_STATEMENT_H
