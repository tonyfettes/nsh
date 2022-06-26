#ifndef NSH_KEYWORD_H
#define NSH_KEYWORD_H

#include "nsh/string.h"

enum keyword {
  keyword_case,
  keyword_esac,
  keyword_do,
  keyword_done,
  keyword_if,
  keyword_then,
  keyword_elif,
  keyword_else,
  keyword_fi,
  keyword_for,
  keyword_in,
  keyword_while,
  keyword_until,
  keyword_brace_open,
  keyword_brace_close,
  keyword_none,
};

enum keyword keyword_from(struct string string);

#endif // NSH_KEYWORD_H
