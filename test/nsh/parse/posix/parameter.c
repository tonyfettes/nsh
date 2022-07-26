#include "nsh/parse/posix/parameter.h"

#include "common.h"

static bool run_parameter(char const *input,
                          struct parameter *parameter) {
  struct parse parse;
  parse_init(&parse);
  parse_setup(&parse, input);
  parse.parameter_level++;
  bool ok = parse_parameter(&parse, parameter);
  parse_destroy(&parse);
  return ok;
}

static void test_simple() {
  struct parameter param;
  parameter_init(&param);
  assert(run_parameter("param}", &param));
  assert(strcmp(string_data(&param.name), "param") == 0);
  assert(param.type == parameter_simple);
  parameter_destroy(&param);
}

static void test_length() {
  struct parameter param;
  parameter_init(&param);
  assert(run_parameter("#param}", &param));
  assert(strcmp(string_data(&param.name), "param") == 0);
  assert(param.type == parameter_length);
  parameter_destroy(&param);
}

static void test_default() {
  struct parameter param;
  parameter_init(&param);
  assert(run_parameter("param:-hello}", &param));
  assert(strcmp(string_data(&param.name), "param") == 0);
  assert(param.type == parameter_default);
  assert(param.null_as_unset == true);
  parameter_destroy(&param);
}

static void test_pattern() {
  struct parameter param;
  parameter_init(&param);
  assert(run_parameter("param##**}", &param));
  assert(strcmp(string_data(&param.name), "param") == 0);
  assert(param.type == parameter_prefix);
  assert(param.longest_match == true);
  parameter_destroy(&param);
}

int main() {
  test_simple();
  test_length();
  test_default();
  test_pattern();
  return 0;
}
