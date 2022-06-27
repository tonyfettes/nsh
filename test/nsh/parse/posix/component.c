#include "nsh/parse/posix/component.h"
#include "nsh/parse/posix/segment.h"

#include "common.h"

static bool run_component(char const *input,
                          struct component *component) {
  struct parse parse;
  parse_init(&parse);
  parse_setup(&parse, input);
  bool ok = parse_component(&parse, component, true);
  parse_destroy(&parse);
  return ok;
}

static void test_component_redirect() {
  struct component comp;
  component_init(&comp);

  assert(run_component("1>hello.txt", &comp));
  assert(comp.type == component_redirect);
  assert(comp.redirect.source == 1);
  assert(comp.redirect.type == redirect_path);
  struct segment *expan;
  assert(comp.redirect.target.segment.size == sizeof(*expan));
  expan = comp.redirect.target.segment.data;
  assert(expan->type == segment_plain);

  struct string s;
  string_init(&s);
  string_puts(&s, "hello.txt");
  assert(string_equal(expan->plain, s));
  string_destroy(&s);

  assert(run_component("\\>hello.txt", &comp));
  assert(comp.type == component_word);

  assert(run_component("\\1>hello.txt", &comp));
  assert(comp.type == component_word);

  component_destroy(&comp);
}

static void test_component_environment() {
  struct component comp;
  component_init(&comp);
  assert(run_component("ab=c", &comp));
  assert(comp.type == component_environment);
  struct string name;
  string_init(&name);
  string_puts(&name, "ab");
  struct string value;
  string_init(&value);
  string_puts(&value, "c");
  assert(string_equal(comp.environment.name, name));
  string_destroy(&name);
  struct segment *expan;
  assert(comp.environment.value.segment.size == sizeof(*expan));
  expan = comp.environment.value.segment.data;
  assert(expan->type == segment_plain);
  assert(string_equal(value, expan->plain));
  string_destroy(&value);
  component_destroy(&comp);
}

static void test_component_word() {
  struct component comp;
  component_init(&comp);
  assert(run_component("abc", &comp));
  assert(comp.type == component_word);
  struct string s;
  string_init(&s);
  string_puts(&s, "abc");
  struct segment *expan;
  assert(comp.word.segment.size == sizeof(*expan));
  expan = comp.word.segment.data;
  assert(expan->type == segment_plain);
  assert(string_equal(s, expan->plain));
  string_destroy(&s);
  component_destroy(&comp);
}

int main() {
  test_component_redirect();
  test_component_environment();
  test_component_word();
  return 0;
}
