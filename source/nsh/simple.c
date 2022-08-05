#include "nsh/simple.h"

#include "nsh/word.h"
#include "nsh/environment.h"
#include "nsh/variable.h"
#include "nsh/context.h"

void simple_init(struct simple *simple) {
  simple->expanded = false;
  stack_init(&simple->argument);
  stack_init(&simple->assignment);
}

static void simple_content_clear(struct simple *simple) {
  void *head;
  void *tail;
  if (simple->expanded) {
    head = stack_head(&simple->argument, 0);
    tail = stack_tail(&simple->argument, 0);
    for (struct word *word = head; word != tail; word++) {
      word_destroy(word);
    }
    head = stack_head(&simple->assignment, 0);
    tail = stack_tail(&simple->assignment, 0);
    for (struct assignment *assign = head; assign != tail; assign++) {
      assignment_destroy(assign);
    }
  } else {
    head = stack_head(&simple->argument, 0);
    tail = stack_tail(&simple->argument, 0);
    for (struct string *arg = head; arg != tail; arg++) {
      string_destroy(arg);
    }
    head = stack_head(&simple->environment, 0);
    tail = stack_tail(&simple->environment, 0);
    for (struct environment *env = head; env != tail; env++) {
      environment_destroy(env);
    }
  }
}

void simple_clear(struct simple *simple) {
  simple_content_clear(simple);
  stack_clear(&simple->argument);
  stack_clear(&simple->assignment);
}

void simple_destroy(struct simple *simple) {
  simple_content_clear(simple);
  stack_destroy(&simple->argument);
  stack_destroy(&simple->environment);
}

// static bool simple_is_empty(struct simple simple) {
//   return simple.expanded == false &&
//          simple.argument.size == 0 &&
//          simple.assignment.size == 0;
// }

bool simple_expand(struct simple simple, struct simple *target) {
  assert(simple.expanded == false);
  simple_clear(target);
  static struct string const ifs_key = {
    .capacity = 4,
    .size = 3,
    .stack = "IFS",
  };
  struct string ifs;
  string_init(&ifs);
  hash_size_t index;
  if (!hash_find(&context.variable, ifs_key, &index)) {
    if (!string_puts(&ifs, " \t\n\r")) {
      string_destroy(&ifs);
      return false;
    }
  }
  void const *head = stack_head(&simple.argument, 0);
  void const *tail = stack_tail(&simple.argument, 0);
  for (struct word const *word = head; word != tail; word++) {
    if (!word_split(*word, &target->argument, ifs)) {
      return false;
    }
  }
  head = stack_head(&simple.assignment, 0);
  tail = stack_tail(&simple.assignment, 0);
  for (struct assignment const *assign = head; assign != tail;
       assign++) {
    if (!stack_alloc(&target->environment,
                     sizeof(struct environment))) {
      return false;
    }
    struct environment *env;
    env = stack_tail(&target->environment, 0);
    environment_init(env);
    if (!assignment_expand(*assign, env)) {
      return false;
    }
    assert(stack_bump(&target->environment,
                      sizeof(struct environment)));
  }
  target->expanded = true;
  return true;
}

bool simple_display(struct simple simple, struct string *target) {
  for (struct environment *env = stack_head(&simple.environment, 0);
       env != stack_tail(&simple.environment, 0); env++) {
    if (target->size) {
      try(string_putc(target, ' '));
    }
    try(environment_display(env, target));
  }
  for (struct word *arg = stack_head(&simple.argument, 0);
       arg != stack_head(&simple.argument, 0); arg++) {
    if (!word_expand(*arg, target)) {
      return false;
    }
  }
  return true;
}

bool simple_execute(struct simple simple, struct context *context,
                    struct stack redirect, struct pipe *pipe) {
  assert(simple.expanded);
  struct string *name = stack_head(&simple.argument, 0);
  hash_size_t index;
  if (hash_find(context->builtin, *name, &index)) {
    struct builtin *builtin = hash_head(&context->builtin, index, 0);
    builtin->main(context, simple);
  } else if (hash_find(context->function, *name, &index)) {
    struct command *body = hash_head(&context->function, index, 0);
    if (!command_execute(*body, context, pipe)) {
      return false;
    }
  } else {
    struct string path;
    string_init(&path);
    struct job *job = stack_tail(&context->job, sizeof(struct job));
    struct string *directory = stack_tail(&context->directory, 0);
    if (!group_spawn(&job->group, *directory, path, simple, redirect,
                     pipe)) {
      string_destroy(&path);
      return false;
    }
  }
  return true;
}
