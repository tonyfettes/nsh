#ifndef NSH_PARSE_REDIRECT_H
#define NSH_PARSE_REDIRECT_H

#include "nsh/parse/common.h"
#include "nsh/redirect.h"
#include "nsh/parse/word.h"
#include "nsh/command.h"

bool parse_redirect_source(struct parse *parse, int *source,
                           struct string *string);

bool parse_redirect(struct parse *parse, struct redirect *redirect);

bool parse_redirect_list(struct parse *parse,
                         struct stack *redirect_list);

#endif /* NSH_PARSE_REDIRECT_H */
