#ifndef NSH_PARSE_ENVIRONMENT_H
#define NSH_PARSE_ENVIRONMENT_H

#include "nsh/parse/posix/common.h"
#include "nsh/environment.h"

// Parse an environment assignment, store word in `value` with `name`
// set to empty to indicate failure.
bool parse_environment(struct parse *restrict parse,
                       struct environment *restrict environment);

#endif /* NSH_PARSE_ENVIRONMENT_H */
