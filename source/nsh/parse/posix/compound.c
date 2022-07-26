#include "nsh/parse/posix/compound.h"
#include "nsh/parse/posix/delimiter.h"

bool parse_subshell(struct parse *parse, struct subshell *subshell) {
  (void) parse, (void) subshell;
  todo("Parsing subshell compound list");

  char c;
  parse_peek(parse, &c);
  while (!is_delimiter(parse, c)) {
    if (!parse_block(parse, subshell->body, 
    if (!stack_alloc(&subshell->body, sizeof(struct command))) {
      return false;
    }
  }
}

bool parse_brace(struct parse *parse, struct brace *brace) {
  (void) parse, (void) brace;
  todo("Parsing brace compound list");
}
