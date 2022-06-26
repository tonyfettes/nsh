#ifndef NSH_FILE_H
#define NSH_FILE_H

#include "nsh/common.h"
#include "nsh/string.h"

struct file {
  int fd;
};

bool file_open(struct file *file, struct string path, int flags,
               mode_t mode);

bool file_close(struct file *file);

#endif // NSH_FILE_H
