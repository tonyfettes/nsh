#include "nsh/process.h"

#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>

void group_init(struct group *group) {
  group->foreground = false;
  group->leader.pid = -1;
  stack_init(&group->member);
}

void group_destroy(struct group *group) {
  stack_destroy(&group->member);
}

typedef posix_spawn_file_actions_t fact_t;

static bool fact_init(fact_t *fact) {
  int rc = posix_spawn_file_actions_init(fact);
  return (rc == 0) ? true : (throw_system(rc), false);
}

static void fact_destroy(fact_t *fact) {
  assert(posix_spawn_file_actions_destroy(fact) == 0);
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

static bool fact_tcsetpgrp(fact_t *fact) {
  int rc = posix_spawn_file_actions_addtcsetpgrp_np(fact, STDIN_FILENO);
  return (rc == 0) ? true : (throw_system(rc), false);
}

typedef posix_spawnattr_t attr_t;

static bool attr_init(attr_t *attr) {
  int rc = posix_spawnattr_init(attr);
  return (rc == 0) ? true : (throw_system(rc), false);
}

static void attr_destroy(attr_t *attr) {
  assert(posix_spawnattr_destroy(attr) == 0);
}

static bool spawn(pid_t *restrict pid, char const *restrict path,
                  const fact_t *restrict fact,
                  const attr_t *restrict attr,
                  char *const argv[restrict],
                  char *const envp[restrict]) {
  int rc = posix_spawnp(pid, path, fact, attr, argv, envp);
  return (rc == 0) ? true : (throw_system(rc), false);
}

struct open_file {
  int descriptor;
  unsigned int flags;
};

static struct open_file const open_std[] = {
  [STDIN_FILENO] = { .descriptor = STDIN_FILENO, .flags = O_RDONLY },
  [STDOUT_FILENO] = {
    .descriptor = STDOUT_FILENO, .flags = O_WRONLY
  },
  [STDERR_FILENO] = {
    .descriptor = STDERR_FILENO, .flags = O_WRONLY
  },
};

static bool setup_fact_handle_descriptor(fact_t *fact,
                                         struct stack *open,
                                         struct redirect redirect) {
  struct open_file *head = stack_head(open, 0);
  struct open_file *tail = stack_tail(open, 0);
  struct open_file *target = NULL;
  struct open_file *source = NULL;
  for (struct open_file *file = head; file != tail; file++) {
    if (redirect.source == file->descriptor) {
      source = file;
      if (redirect.descriptor == -1) {
        try(fact_close(fact, redirect.source));
        stack_remove(open, source - head, NULL,
                     sizeof(struct open_file));
      }
    }
    if (redirect.descriptor == file->descriptor) {
      if (file->flags & O_RDWR) {
        target = file;
      }
      if ((redirect.flags & O_RDONLY) &&
          (file->flags & O_RDONLY)) {
        target = file;
      }
      if ((redirect.flags & O_WRONLY) &&
          (file->flags & O_WRONLY)) {
        target = file;
      }
    }
    if (source && target) {
      break;
    }
  }
  if (!target) {
    todo("Diagnose when target file descriptor is not open.");
  }
  if (redirect.descriptor == redirect.source) {
    return true;
  }
  try(fact_dup2(fact, redirect.descriptor, redirect.source));
  if (source) {
    stack_remove(open, source - head, NULL, 0);
  } else {
    struct open_file newly_open = {
      .descriptor = redirect.source,
      .flags = target->flags,
    };
    stack_push(open, &newly_open, sizeof(struct open_file));
  }
  return true;
}

static bool setup_fact_handle_path(fact_t *fact, struct stack *open,
                                   struct redirect redirect) {
  struct open_file *head = stack_head(open, 0);
  struct open_file *tail = stack_tail(open, 0);
  struct open_file *source = NULL;
  for (struct open_file *file = head; file != tail; file++) {
    if (file->descriptor == redirect.source) {
      source = file;
      break;
    }
  }
  try(fact_open(fact, redirect.source, redirect.path,
                redirect.flags, 0644));
  if (source) {
    source->flags = redirect.flags;
  } else {
    struct open_file newly_open = {
      .descriptor = redirect.source,
      .flags = redirect.flags,
    };
    stack_push(open, &newly_open, sizeof(struct open_file));
  }
  return true;
}

static bool setup_fact(fact_t *fact,
                       struct stack redirect_stack,
                       struct pipe *pipe,
                       bool control) {
  if (control) {
    try(fact_tcsetpgrp(fact));
  }
  if (pipe->read.fd != -1) {
    try(fact_dup2(fact, pipe->read.fd, STDIN_FILENO));
    try(fact_close(fact, pipe->read.fd));
    pipe->read.fd = -1;
  }
  if (pipe->write.fd != -1) {
    try(fact_dup2(fact, pipe->write.fd, STDOUT_FILENO));
    try(fact_close(fact, pipe->write.fd));
    pipe->write.fd = -1;
  }
  struct stack open;
  stack_init(&open);
  if (!stack_push(&open, open_std, sizeof(open_std))) {
    return false;
  }
  for (struct redirect
       *redirect = stack_head(&redirect_stack, 0),
       *tail = stack_tail(&redirect_stack, 0);
       redirect != tail; redirect++) {
    switch (redirect->type) {
    case redirect_descriptor:
      try(setup_fact_handle_descriptor(fact, &open, *redirect));
      break;
    case redirect_path:
      try(setup_fact_handle_path(fact, &open, *redirect));
      break;
    case redirect_heredoc:
      todo("Here-document");
      // TODO:
      break;
    }
  }
  return true;
}

static bool setup_attr(attr_t *attr) {
  int rc = posix_spawnattr_setflags(attr, POSIX_SPAWN_SETPGROUP);
  return rc ? (throw_system(rc), false) : true;
}

static bool setup_argv(struct stack *argp, struct simple simple) {
  struct string *head = stack_head(&simple.argument, 0);
  struct string *tail = stack_tail(&simple.argument, 0);
  for (struct string *args = head; args != tail; args++) {
    // When memory allocation failed here, we will not destroy
    // previously allocated strings here. They will be taken care in
    // main execute function.
    if (!stack_alloc(argp, sizeof(char *))) {
      return false;
    }
    char **data = stack_tail(argp, 0);
    *data = string_data(args);
    assert(stack_bump(argp, sizeof(char *)));
  }
  char *null = NULL;
  return stack_push(argp, &null, sizeof(char *));
}

static bool setup_envp(struct stack *envp, struct stack *envs,
                       struct program program) {
  for (char **env = environ; *env; env++) {
    stack_push(envp, env, sizeof(char *));
  }
  for (struct environment *in = stack_head(&program.environment, 0);
       in != stack_tail(&program.environment, 0); in++) {
    // Allocate, initialize environment string as name=value format.
    if (!stack_alloc(envs, sizeof(struct string))) {
      return false;
    }
    struct string *new = stack_tail(envs, 0);
    string_init(new);
    if (!(string_concat(new, in->name) &&
          string_putc(new, '=') &&
          string_concat(new, in->value))) {
      string_destroy(new);
      return false;
    }
    // Search through envp and replace/append environment variables.
    bool found = false;
    for (char **out = stack_head(envp, 0); out != stack_tail(envp, 0);
         out++) {
      size_t len = strchr(*out, '=') - *out;
      if (len != in->name.size ||
          strncmp(*out, string_data(&in->name), len) != 0) {
        continue;
      }
      *out = string_data(new);
      found = true;
      break;
    }
    if (!found) {
      if (!stack_alloc(envp, sizeof(char *))) {
        string_destroy(new);
        return false;
      }
      char **out = stack_tail(envp, 0);
      *out = string_data(new);
      assert(stack_bump(envp, sizeof(char *)));
    }
    assert(stack_bump(envs, sizeof(struct string)));
  }
  char *null = NULL;
  return stack_push(envp, &null, sizeof(char *));
}

bool group_spawn(struct group *group, struct string path,
                 struct simple simple, struct stack redirect_stack,
                 struct pipe *pipe) {
  if (!stack_alloc(&group->member, sizeof(struct process))) {
    return false;
  }
  struct process *process;
  process = stack_tail(&group->member, sizeof(struct process));

  bool ok = true;
  fact_t fact;
  try(fact_init(&fact));
  bool fact_control = false;
  if (group->foreground == true && group->leader.pid == -1) {
    fact_control = true;
  }
  if (!(ok = setup_fact(&fact, redirect_stack, pipe, fact_control))) {
    goto return_fact;
  }

  struct stack argt;
  stack_init(&argt);
  if (!(ok = setup_argv(&argt, program))) {
    goto return_argv;
  }
  char *const *argv = stack_head(&argt, 0);

  struct stack envt;
  stack_init(&envt);
  struct stack envs;
  stack_init(&envs);
  if (!(ok = setup_envp(&envt, &envs, program))) {
    goto return_envp;
  }
  char *const *envp = stack_head(&envt, 0);

  attr_t attr;
  if (!(ok = attr_init(&attr))) {
    goto return_envp;
  }
  if (!(ok = setup_attr(&attr))) {
    goto return_envp;
  }

  ok = spawn(&process->pid, string_data(&program.path), &fact, &attr,
             argv, envp);

  group->leader.pid = process->pid;

  attr_destroy(&attr);

return_envp:
  stack_destroy(&envt);
  for (struct string *env = stack_head(&envs, 0);
       env != stack_tail(&envs, 0); env++) {
    string_destroy(env);
  }
  stack_destroy(&envs);

return_argv:
  stack_destroy(&argt);

return_fact:
  fact_destroy(&fact);
  return ok;
}

bool process_fork(struct process *process, bool (*func)(void *data),
                  void *data, struct stack redirect, struct pipe *pipe,
                  unsigned int flags) {
  pid_t pid = fork();
  if (pid == -1) {
    throw_system(errno);
    return false;
  }

  if (pid == 0) {
    if (!func(data)) {
      exit(EXIT_FAILURE);
    }
  } else {
    process->pid = pid;
  }
  return true;
}
