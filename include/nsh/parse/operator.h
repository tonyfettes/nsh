#ifndef NSH_PARSE_OPERATOR_H
#define NSH_PARSE_OPERATOR_H

#include <stdbool.h>

bool is_operator(int ch);

bool is_control_operator(int ch);

bool is_redirect_operator(int ch);

#endif /* NSH_PARSE_OPERATOR_H */
