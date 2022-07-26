#ifndef NSH_PARSE_COMMON_H
#define NSH_PARSE_COMMON_H

#include "nsh/string.h"
#include "nsh/diagnosis.h"
#include "nsh/parse.h"
#include <ctype.h>

// struct track {
//   struct string name;
//   struct string value;
//   size_t index;
// };
//
// void track_init(struct track *track);
//
// void track_destroy(struct track *track);
//
// struct parse {
//   // Stack of `struct track`.
//   struct stack track;
//   struct location location;
//   // Only parameter expansion would make unquoted '}' a delimiter.
//   size_t parameter_level;
//   // Stack of `struct diagnosis'
//   struct stack diagnosis;
// };
//
// extern int error_parse;
//
// enum error_parse {
//   error_parse_invalid_syntax,
// };
//
// void parse_init(struct parse *parse);
//
// void parse_destroy(struct parse *parse);
//
// bool parse_peek(struct parse *parse, char *ch);
//
// bool parse_bump(struct parse *parse, size_t size);
//
// bool parse_bump_peek(struct parse *parse, char *ch);

#endif // NSH_PARSE_COMMON_H
