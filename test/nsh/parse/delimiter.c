#include "nsh/parse/delimiter.h"

#include <stddef.h>
#include <stdio.h>

static void test_single_blank() {
  char const line[] = "hi, w";
  char const *cptr = line;
  assert(!parse_delimiter(&cptr, NULL));
  assert(cptr == line);
  cptr += 3;
  assert(parse_delimiter(&cptr, NULL));
  assert(*cptr == 'w');
}

static void test_multiple_blanks() {
  char const line[] = "hi,   \t\t\t   \tw";
  char const *cptr = line;
  assert(!parse_delimiter(&cptr, NULL));
  assert(cptr == line);
  cptr += 3;
  assert(parse_delimiter(&cptr, NULL));
  assert(*cptr == 'w');
}

static void test_operator() {
  char const *lines[] = {
    "hi#w",
    "hi\nw",
    "hi",
    "hi<w",
    "hi>w",
    "hi|w",
    "hi&w",
    "hi(w",
    "hi)w",
    "hi;w",
  };
  for (size_t i = 0; i < sizeof(lines) / sizeof(lines[0]); i++) {
    char const *cptr = lines[i] + 2;
    assert(parse_delimiter(&cptr, NULL));
    assert(*cptr == lines[i][2]);
  }
}

int main() {
  test_single_blank();
  test_multiple_blanks();
  test_operator();
  return 0;
}
