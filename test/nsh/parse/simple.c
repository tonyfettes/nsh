#include "nsh/parse/simple.h"

#include "common.h"

static bool run_simple(char const *input, struct simple *simple,
                       struct stack *redirect) {
  struct parse parse;
  parse_init(&parse);
  parse_setup(&parse, input);
  bool ok = parse_simple(&parse, simple, redirect);
  parse_destroy(&parse);
  return ok;
}

static void test_plain() {
  struct simple simple;
  simple_init(&simple);

  struct stack redirect;
  stack_init(&redirect);

  assert(run_simple("AB=C > hello CC=gcc echo 123 CXX=g++ < why 456",
                    &simple, &redirect));
  assert(simple.argument.size == sizeof(struct word) * 4);
  assert(simple.environment.size == sizeof(struct environment) * 2);
  assert(redirect.size == sizeof(struct redirect) * 2);

  struct redirect *r = redirect.data;
  size_t size = redirect.size / sizeof(struct redirect);
  for (size_t i = 0; i < size; i++) {
    redirect_destroy(&r[i]);
  }

  stack_destroy(&redirect);
  simple_destroy(&simple);
}

int main() {
  test_plain();
  return 0;
}
