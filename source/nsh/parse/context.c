#include "context.h"

extern void context_init(struct context *restrict context) {
  stack_init(&context->diagnosis, 0);
}

extern void context_save(struct context const *restrict context,
                         struct snapshot *restrict snapshot) {
  snapshot->cptr = context->cptr;
  snapshot->diagnosis_size = context->diagnosis.size;
}

extern void context_restore(struct context *restrict context,
                            struct snapshot const *restrict snapshot) {
  context->cptr = snapshot->cptr;
  context->diagnosis.size = snapshot->diagnosis_size;
}

extern bool context_merge(struct context const *restrict context,
                          struct snapshot const *restrict snapshot,
                          struct context *restrict error) {
  if (context->cptr > error->cptr) {
    error->cptr = context->cptr;
    stack_clear(&error->diagnosis);
    stack_push(&error->diagnosis,
               context->diagnosis.data + snapshot->diagnosis_size,
               context->diagnosis.size - snapshot->diagnosis_size);
  }
  return true;
}

extern bool context_update(struct context *restrict context,
                           struct context const *restrict error) {
  if (!stack_push(&context->diagnosis, error->diagnosis.data,
                  error->diagnosis.size)) {
    return false;
  }
  context->cptr = error->cptr;
  return true;
}

extern void context_destroy(struct context *restrict context) {
  stack_destroy(&context->diagnosis);
}
