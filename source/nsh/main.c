#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include "nsh/parse.h"
#include "nsh/context.h"

int main(int argc, char const *argv[]) {
  (void) argc;
  (void) argv;

  setlocale(LC_ALL, "");

  struct parse parse;
  parse_init(&parse);
  if (!stack_alloc(&parse.track, sizeof(struct track))) {
    return EXIT_FAILURE;
  }
  struct track *track = stack_tail(&parse.track, 0);
  track_init(track);

  // Let `getline()` allocate buffer for us.
  char *line = NULL;
  size_t line_capacity = 0;
  ssize_t line_size;

  fprintf(stdout, "> ");

  while ((line_size = getline(&line, &line_capacity, stdin)) != -1) {
    string_place(&track->value, line, line_size, line_capacity);
    // Parsing...
    // Executing...
    fprintf(stdout, "> ");
  }

  context_init(&context);
  while (true) {
    if (!context_poll(&context)) {
      // diagnosis.
    }
  }
  context_destroy(&context);

  return EXIT_SUCCESS;
}
