#include "nsh/job.h"
#include "nsh/command.h"
#include "nsh/builtin.h"
#include "nsh/variable.h"
#include "nsh/context.h"
#include "nsh/hash.h"
#include <sys/syscall.h>

struct pool {
  unsigned int next;
  struct stack recycled;
};

static struct pool pool = {
  .next = 0,
  .recycled = {
    .size = 0,
    .data = NULL,
    .capacity = 0,
  },
};

static bool job_id_alloc(unsigned int *id) {
  if (pool.recycled.size != 0) {
    stack_pop(&pool.recycled, id, sizeof(unsigned int));
  } else {
    assert(pool.next >= pool.recycled.size);
    try(stack_alloc(&pool.recycled, pool.next - pool.recycled.size));
    *id = pool.next;
    pool.next++;
  }
  return true;
}

static void job_id_free(unsigned int *id) {
  assert(stack_push(&pool.recycled, id, sizeof(unsigned int)));
  *id = 0;
}

bool job_init(struct job *job) {
  if (!job_id_alloc(&job->id)) {
    return false;
  }
  string_init(&job->name);
  group_init(&job->group);
  return true;
}

void job_destroy(struct job *job) {
  job_id_free(&job->id);
  string_destroy(&job->name);
  group_destroy(&job->group);
}

static struct string const ifs_key = {
  .capacity = 4,
  .size = 3,
  .stack = "IFS",
};


bool job_display(struct job *job, struct string *target) {
}
