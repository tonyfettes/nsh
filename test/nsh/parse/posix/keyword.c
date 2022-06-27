#include "nsh/keyword.h"

int main() {
  struct string string;
  string_init(&string);
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
  for (size_t i = 0; i < sizeof(table) / sizeof(table[0]); i++) {
    string_puts(&string, table[i]);
    assert(keyword_from(string) == i);
    string_clear(&string);
  }
  string_puts(&string, "hahaha");
  assert(keyword_from(string) == keyword_none);
  string_destroy(&string);
  return 0;
}
