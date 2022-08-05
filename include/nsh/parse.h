#ifndef NSH_PARSE_H
#define NSH_PARSE_H

#include "nsh/common.h"
#include "nsh/string.h"

struct track {
  // Name of current alias being expanded.
  struct string name;
  // Content of current alias.
  struct string value;
  // Position of read head.
  string_size_t index;
};

void track_init(struct track *track);

void track_destroy(struct track *track);

struct parse {
  struct context *context;
  // Stack of `struct parse_track`.
  struct stack track;
  // Location of read head in original text.
  struct location location;
  // Only parameter expansion would make unquoted '}' a delimiter.
  size_t parameter_level;
  // Stack of `struct diagnosis`.
  struct stack diagnosis;
};

enum parse_error {
  parse_error_invalid_syntax,
};

bool parse_error(struct string *target, int code);

void parse_init(struct parse *parse);

void parse_destroy(struct parse *parse);

// Copy `size` bytes content into `target`, starting from current
// location. The number of bytes read is written to `size`.
void parse_peek_by(struct parse *parse, char *restrict target,
                   string_size_t *size);

void parse_peek(struct parse *parse, char *restrict ch);

bool parse_bump_by(struct parse *parse, string_size_t size);

bool parse_bump(struct parse *parse);

bool parse_bump_peek(struct parse *parse, char *restrict ch);

bool parse_read(struct parse *parse, char *restrict target,
                string_size_t *size);

__attribute__((format(printf, 3, 4)))
void parse_log(struct parse parse, enum log_level level,
               char const *restrict format, ...);

#endif // NSH_PARSE_H
