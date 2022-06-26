#include "nsh/keyword.h"

#include <string.h>

enum keyword keyword_from(struct string string) {
  static char const *const table[] = {
    [keyword_case] = "case",
    [keyword_esac] = "esac",
    [keyword_do] = "do",
    [keyword_done] = "done",
    [keyword_if] = "if",
    [keyword_then] = "then",
    [keyword_elif] = "elif",
    [keyword_else] = "else",
    [keyword_fi] = "fi",
    [keyword_for] = "for",
    [keyword_in] = "in",
    [keyword_while] = "while",
    [keyword_until] = "until",
    [keyword_brace_open] = "{",
    [keyword_brace_close] = "}",
  };
  static const size_t size = sizeof(table) / sizeof(table[0]);
  char const *data = string_data(&string);
  for (size_t i = 0; i < size; i++) {
    if (strcmp(data, table[i]) == 0) {
      return i;
    }
  }
  return keyword_none;
}
