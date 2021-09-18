#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct {
  int in;
  int out;
  int err;
} io_stream_t;

typedef struct {
  pid_t pid;
  io_stream_t streams;
} process_t;

#endif // PROCESS_H
