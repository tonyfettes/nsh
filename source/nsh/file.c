#include "nsh/file.h"

bool file_open(struct file *file, struct string path, int flags,
               mode_t mode) {
  int fd = open(string_data(&path), flags | O_CLOEXEC, mode);
  if (fd == -1) {
    throw_system(errno);
    return false;
  }
  file->fd = fd;
  return true;
}

bool file_close(struct file *file) {
  int rc = close(file->fd);
  if (rc == -1) {
    assert(errno != EBADF);
    throw_system(errno);
    return false;
  }
  file->fd = -1;
  return true;
}

bool pipe_open(struct pipe *pipe_wrapper) {
  int pipe_file_descriptor[2];
  int rc = pipe(pipe_file_descriptor);
  pipe_wrapper->read.fd = pipe_file_descriptor[0];
  pipe_wrapper->write.fd = pipe_file_descriptor[1];
  if (rc != 0) {
    throw_system(errno);
    return false;
  }
  return true;
}

bool directory_open(struct directory *directory, struct string name) {
  DIR *stream = opendir(string_data(&name));
  if (!stream) {
    throw_system(errno);
    return false;
  }
  directory->stream = stream;
  return true;
}

void directory_close(struct directory *directory) {
  // The only possible error is invalid stream, and could only be caused
  // by bugs, so we assert the call to succeed.
  assert(closedir(directory->stream) == 0);
}
