#ifndef NSH_PARSE_FUNCTION_H
#define NSH_PARSE_FUNCTION_H

#include "nsh/parse/posix/common.h"
#include "nsh/function.h"

bool parse_function(struct parse *parse, struct function *function,
                    struct redirect_list *redirect);

#endif // NSH_PARSE_FUNCTION_H
