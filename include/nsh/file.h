#ifndef NSH_FILE_H
#define NSH_FILE_H

#include "nsh/common.h"
#include "nsh/string.h"
#include <dirent.h>

struct file {
  int fd;
};

bool file_open(struct file *file, struct string path, int flags,
               mode_t mode);

bool file_close(struct file *file);

struct file_stdio {
  struct file in;
  struct file out;
  struct file err;
};

static struct file_stdio const file_stdio = {
  .in = { STDIN_FILENO },
  .out = { STDOUT_FILENO },
  .err = { STDERR_FILENO },
};

struct pipe {
  struct file read;
  struct file write;
};

bool pipe_open(struct pipe *pipe);

bool pipe_close(struct pipe *pipe);

struct directory {
  DIR *stream;
};

bool directory_open(struct directory *directory, struct string name);

void directory_close(struct directory *directory);

#endif // NSH_FILE_H
