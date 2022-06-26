#include "nsh/execute/simple.h"

#include <sys/wait.h>

static void test_plain() {
  char const *const argv[] = {
    "env",
  };

  struct simple simple;
  simple_init(&simple);
  for (size_t i = 0; i < sizeof(argv) / sizeof(argv[0]); i++) {
    struct word word;
    word_init(&word);
    word.type = word_plain;
    string_init(&word.plain);
    assert(string_puts(&word.plain, argv[i]));
    assert(stack_push(&simple.argument, &word, sizeof(word)));
  }

  struct environment env;
  environment_init(&env);
  assert(string_puts(&env.name, "AB"));
  env.value.type = word_plain;
  string_init(&env.value.plain);
  string_puts(&env.value.plain, "C");
  assert(stack_push(&simple.environment, &env, sizeof(env)));

  struct stack redirect;
  stack_init(&redirect);

  assert(execute_simple(&simple, &redirect));

  int status;
  assert(waitpid(0, &status, 0) != -1);

  stack_destroy(&redirect);

  simple_destroy(&simple);
}

int main() {
  test_plain();
  return 0;
}
