#include "nsh/parse/command.h"
#include "nsh/context.h"
#include "nsh/redirect.h"

#include "common.h"

static bool run_command(char const *input, struct command *command) {
  struct parse parse;
  parse_init(&parse);
  parse_setup(&parse, input);
  bool ok = parse_command(&parse, command);
  parse_destroy(&parse);
  return ok;
}

static void test_alias() {
  context_init(&context);

  struct string alias_name;
  string_init(&alias_name);
  assert(string_puts(&alias_name, "ll"));

  struct string alias_value;
  string_init(&alias_value);
  assert(string_puts(&alias_value, "AB=gcc ls > hello.txt -lha"));

  assert(alias_table_insert(&context.alias_table, &alias_name,
                            &alias_value));

  struct command command;
  command_init(&command);

  assert(run_command("ll --color", &command));
  assert(command.type == command_simple);
  assert(command.redirect.size == sizeof(struct redirect) * 1);
  assert(command.simple.argument.size == sizeof(struct word) * 3);
  assert(command.simple.environment.size == sizeof(struct environment));

  command_destroy(&command);

  string_destroy(&alias_name);

  string_destroy(&alias_value);

  context_destroy(&context);
}

int main() {
  test_alias();
  return 0;
}
