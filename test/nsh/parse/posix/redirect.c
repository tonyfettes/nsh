#include "nsh/parse/posix/redirect.h"

#include "common.h"
#include <stdio.h>

static int run_redirect_source(char const *input) {
  struct parse parse;
  parse_init(&parse);
  parse_setup(&parse, input);
  int source = -1;
  struct string string;
  string_init(&string);
  bool ok = parse_redirect_source(&parse, &source, &string);
  string_destroy(&string);
  parse_destroy(&parse);
  if (!ok) {
    return -1;
  } else {
    return source;
  }
}

static void test_redirect_source_zero() {
  assert(run_redirect_source("0") == -1);
  assert(run_redirect_source("0>") == 0);
  assert(run_redirect_source("0<") == 0);
  assert(run_redirect_source("0>|") == 0);
  assert(run_redirect_source("0>&") == 0);
  assert(run_redirect_source("0<&") == 0);
  assert(run_redirect_source("0>>") == 0);
  assert(run_redirect_source("0<<") == 0);
  assert(run_redirect_source("0<<-") == 0);
  assert(run_redirect_source("0&") == -1);
  assert(run_redirect_source("0|") == -1);
}

static void test_redirect_source_positive() {
  assert(run_redirect_source("1") == -1);
  assert(run_redirect_source("2>") == 2);
  assert(run_redirect_source("3<") == 3);
  assert(run_redirect_source("4>|") == 4);
  assert(run_redirect_source("5>&") == 5);
  assert(run_redirect_source("6<&") == 6);
  assert(run_redirect_source("7>>") == 7);
  assert(run_redirect_source("8<<") == 8);
  assert(run_redirect_source("9<<-") == 9);
  assert(run_redirect_source("12>") == 12);
}

// static void test_negative() {
//   test("-1", false, 0);
// }

static void test_redirect_source_invalid() {
  assert(run_redirect_source("012>") == -1);
  assert(run_redirect_source("1a>") == -1);
  assert(run_redirect_source("0x12>") == -1);
}

static void test_redirect_source_quoted() {
  // test("\\1>", 0);
  assert(run_redirect_source("1\\>") == -1);
  assert(run_redirect_source("1\\2>") == -1);
}

#if 0
static void test_redirect_source_overflow() {
  assert(run_redirect_source("123412341234123412341234") == 0);
}

static void test_redirect_source_open_max() {
  char line[20];
  snprintf(line, sizeof(line), "%ld", sysconf(_SC_OPEN_MAX));
  assert(run_redirect_source(line) == 0);
}
#endif

static void test_redirect_source() {
  test_redirect_source_zero();
  test_redirect_source_positive();
  test_redirect_source_invalid();
  test_redirect_source_quoted();
}

static bool run_redirect(char const *input, struct redirect *redirect) {
  struct parse parse;
  parse_init(&parse);
  parse_setup(&parse, input);
  if (!parse_redirect(&parse, redirect)) {
    parse_destroy(&parse);
    return false;
  }
  parse_destroy(&parse);
  return true;
}

static void test_redirect_plain() {
  struct redirect redirect;
  redirect_init(&redirect);
  run_redirect(">hello.txt", &redirect);
  assert(redirect.flags |  O_WRONLY);
  assert(redirect.type == redirect_path);
  redirect_destroy(&redirect);
}

static void test_redirect_blank() {
  struct redirect redirect;
  redirect_init(&redirect);
  run_redirect("> hello.txt", &redirect);
  assert(redirect.flags |  O_WRONLY);
  assert(redirect.type == redirect_path);
  redirect_destroy(&redirect);
}

static void test_redirect() {
  test_redirect_plain();
  test_redirect_blank();
}

int main() {
  test_redirect_source();
  test_redirect();
  return 0;
}
