#include "condition.h"

#include "pipeline.h"
#include "delimiter.h"

static bool parse_operator(struct context *restrict context,
                           bool *restrict status) {
  struct snapshot snapshot;
  context_save(context, &snapshot);
  char c[2] = { peek(context), peek(context) };
  if (c[0] == '&' && c[1] == '&') {
    *status = true;
    return true;
  }
  if (c[0] == '|' && c[1] == '|') {
    *status = false;
    return true;
  }
  context_restore(context, &snapshot);
  return false;
}

extern bool parse_condition(struct context *restrict context,
                            struct condition *restrict condition) {
  do {
    struct condition_unit unit;
    if (!parse_pipeline(context, &unit.pipeline)) {
      return false;
    }
    parse_delimiter(context);
    if (!parse_operator(context, &unit.status)) {
      return false;
    }
    parse_delimiter(context);
  } while (true);
}
