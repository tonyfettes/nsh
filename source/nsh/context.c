#include "nsh/context.h"
#include <sys/stat.h>
#include <unistd.h>

static inline bool get_path_max(int curfd, long *path_max) {
#ifdef PATH_MAX
  (void) curfd;
  *path_max = PATH_MAX;
#elifdef _POSIX_PATH_MAX
  (void) curfd;
  *path_max = _POSIX_PATH_MAX;
#else
  *path_max = fpathconf(curfd, _PC_PATH_MAX);
  if (*path_max == -1) {
    throw_system(errno);
    return false;
  }
#endif
  return true;
}

bool context_init(struct context *context) {
  // Open current directory.
  int curfd = open(".", O_RDONLY | O_DIRECTORY);
  if (curfd == -1) {
    throw_system(errno);
    return false;
  }
  // Get current umask.
  context->umask = umask(0);
  umask(context->umask);
  option_init(&context->option);
  hash_init(&context->alias);
  hash_init(&context->variable);
  hash_init(&context->builtin);
  hash_init(&context->function);
  stack_init(&context->job);
  stack_init(&context->command);
  stack_init(&context->directory);
  // Get current working directory.
  if (!stack_alloc(&context->directory, sizeof(struct string))) {
    return false;
  }
  struct string *directory = stack_tail(&context->directory, 0);
  string_init(directory);
  long path_max;
  if (!get_path_max(curfd, &path_max)) {
    return false;
  }
  if (!string_reserve(directory, path_max + 1)) {
    return false;
  }
  if (getcwd(string_data(directory), path_max + 1) == NULL) {
    throw_system(errno);
    return false;
  }
  context->file.stdin.fd = STDIN_FILENO;
  context->file.stdout.fd = STDOUT_FILENO;
  context->file.stderr.fd = STDERR_FILENO;
  context->file.sink.fd = open("/dev/null", O_RDWR);
  if (context->file.sink.fd == -1) {
    throw_system(errno);
    return false;
  }
  return true;
}

bool context_fork(struct context *target, struct context source) {
  target->umask = source.umask;
  target->option = source.option;
  // Disable job control in sub shell.
  target->option.job.control = false;
  target->option.job.notify = job_option_notify_none;
  try(hash_assign(&target->alias, source.alias));
  try(hash_assign(&target->variable, source.variable));
  try(hash_assign(&target->builtin, source.builtin));
  try(hash_assign(&target->function, source.function));
  try(stack_assign(&target->job, source.job));
  try(stack_assign(&target->directory, source.directory));
  target->file = source.file;
  return true;
}

bool context_destroy(struct context *context, struct context source) {
  if (context->umask != source.umask) {
    umask(source.umask);
    return false;
  }
  hash_destroy(&context->alias);
  hash_destroy(&context->variable);
  hash_destroy(&context->builtin);
  hash_destroy(&context->function);
  stack_destroy(&context->job);
  stack_destroy(&context->command);
  stack_destroy(&context->directory);
  return true;
}
