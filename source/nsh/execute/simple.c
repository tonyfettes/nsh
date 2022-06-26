#include "nsh/execute/simple.h"

#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>

typedef posix_spawn_file_actions_t fact_t;

static bool fact_init(fact_t *fact) {
  int rc = posix_spawn_file_actions_init(fact);
  return (rc == 0) ? true : (throw_system(rc), false);
}

static bool fact_destroy(fact_t *fact) {
  int rc = posix_spawn_file_actions_destroy(fact);
  return (rc == 0) ? true : (throw_system(rc), false);
}

static bool fact_dup2(fact_t *fact, int old, int new) {
  int rc = posix_spawn_file_actions_adddup2(fact, old, new);
  return (rc == 0) ? true : (throw_system(rc), false);
}

static bool fact_open(fact_t *fact, int fd, struct string path,
                      int flags, mode_t mode) {
  int rc = posix_spawn_file_actions_addopen(fact, fd,
                                            string_data(&path), flags,
                                            mode);
  return (rc == 0) ? true : (throw_system(rc), false);
}

static bool fact_close(fact_t *fact, int fd) {
  int rc = posix_spawn_file_actions_addclose(fact, fd);
  return (rc == 0) ? true : (throw_system(rc), false);
}

typedef posix_spawnattr_t attr_t;

static bool attr_init(attr_t *attr) {
  int rc = posix_spawnattr_init(attr);
  return (rc == 0) ? true : (throw_system(rc), false);
}

static bool attr_destroy(attr_t *attr) {
  int rc = posix_spawnattr_destroy(attr);
  return (rc == 0) ? true : (throw_system(rc), false);
}

static bool fact_setup(fact_t *fact, struct stack const *redirect_stack) {
  struct redirect *redirect = redirect_stack->data;
  size_t size = redirect_stack->size / sizeof(struct redirect);
  for (size_t i = 0; i < size; i++) {
    switch (redirect[i].type) {
    case redirect_descriptor:
      try(fact_dup2(fact, redirect[i].descriptor,
                     redirect[i].source));
      try(fact_close(fact, redirect[i].descriptor));
      break;
    case redirect_path:
      try(fact_open(fact, redirect[i].source, redirect[i].path,
                    redirect[i].flags, 0644));
      break;
    case redirect_heredoc:
      // TODO:
      break;
    }
  }
  return true;
}

static bool argv_setup(struct stack *argv_stack,
                       struct simple const *simple) {
  struct word *argument = simple->argument.data;
  size_t size = simple->argument.size / sizeof(struct word);
  char *data;
  for (size_t i = 0; i < size; i++) {
    switch (argument[i].type) {
      struct string *field;
    case word_segment:
      assert(argument[i].type != word_segment);
      break;
    case word_plain:
      data = string_data(&argument[i].plain);
      try(stack_push(argv_stack, &data, sizeof(char *)));
      break;
    case word_field:
      field = argument[i].field.data;
      size = argument[i].field.size;
      for (size_t j = 0; j < size; j++) {
        data = string_data(&field[j]);
        try(stack_push(argv_stack, &data, sizeof(char *)));
      }
      break;
    }
  }
  data = NULL;
  try(stack_push(argv_stack, &data, sizeof(char *)));
  return true;
}

static bool envp_setup(struct stack *envp, struct stack *envs,
                       struct simple const *simple) {
  todo("Setup environment variable before execution");
  for (char **env = environ; *env; env++) {
    stack_push(envp, env, sizeof(char *));
  }
  struct environment *ienv = simple->environment.data;
  char **oenv = envp->data;
  for (size_t i = 0; i < simple->environment.size / sizeof(*ienv); i++) {
    assert(ienv[i].value.type == word_plain);
    bool found = false;
    try(stack_alloc(envs, sizeof(struct string)));
    struct string *nenv = stack_tail(envs, sizeof(struct string));
    string_init(nenv);
    if (!string_append(nenv, ienv[i].name)) {
      string_destroy(nenv);
      return false;
    }
    if (!string_putc(nenv, '=')) {
      string_destroy(nenv);
      return false;
    }
    if (!string_append(nenv, ienv[i].value.plain)) {
      string_destroy(nenv);
      return false;
    }
    for (size_t j = 0; j < envp->size / sizeof(*oenv); j++) {
      size_t len = strchr(oenv[j], '=') - oenv[j];
      if (len != ienv[i].name.size ||
          strncmp(oenv[j], string_data(&ienv[i].name), len) != 0) {
        continue;
      }
      oenv[j] = string_data(nenv);
      assert(stack_bump(envs, sizeof(struct string)));
      found = true;
      break;
    }
    if (!found) {
      if (!stack_alloc(envp, sizeof(char *))) {
        string_destroy(nenv);
        return false;
      }
      char **envp_tail = stack_tail(envp, 0);
      *envp_tail = string_data(nenv);
      assert(stack_bump(envp, sizeof(char *)));
      assert(stack_bump(envs, sizeof(struct string)));
    }
  }
  char *null = NULL;
  if (!stack_push(envp, &null, sizeof(char *))) {
    return false;
  }
}

bool execute_simple(struct simple const *simple,
                    struct stack const *redirect_stack) {
  bool ok = true;
  fact_t fact;
  try(fact_init(&fact));
  if (!(ok = fact_setup(&fact, redirect_stack))) {
    goto return_fact;
  }

  // Stack of `char *`
  struct stack argv_stack;
  stack_init(&argv_stack);
  if (!(ok = argv_setup(&argv_stack, simple))) {
    goto return_argv;
  }
  char *const *argv = argv_stack.data;

  struct stack envp;
  stack_init(&envp);
  struct stack envs;
  stack_init(&envs);
  if (!(ok = envp_setup(&envp, &envs, simple))) {
    goto return_envp;
  }

  attr_t attr;
  try(attr_init(&attr));

  pid_t pid;
  int rc = posix_spawnp(&pid, argv[0], &fact, &attr, argv, envp.data);
  if (rc != 0) {
    ok = false;
    throw_system(rc);
    goto return_attr;
  }

  struct string *env;
  size_t size;

return_attr:
  attr_destroy(&attr);

return_envp:
  env = envs.data;
  size = envs.size / sizeof(struct string);
  for (size_t i = 0; i < size; i++) {
    string_destroy(&env[i]);
  }
  stack_destroy(&envs);
  stack_destroy(&envp);

return_argv:
  stack_destroy(&argv_stack);

return_fact:
  fact_destroy(&fact);
  return ok;
}
