#include "nsh/parse/redirect.h"

#include "nsh/parse/delimiter.h"
#include "nsh/parse/word.h"
#include "nsh/parse/segment.h"

static bool parse_in_dup(struct parse *parse,
                         struct redirect *redirect) {
  try(parse_blank(parse));
  try(parse_word(parse, &redirect->target));
  redirect->type = redirect_descriptor;
  redirect->flags = O_RDONLY;
  return true;
}

static bool parse_in_heredoc(struct parse *parse,
                             struct redirect *redirect) {
  char c;
  try(parse_peek(parse, &c));
  redirect->type = redirect_heredoc;
  redirect->flags = 0;
  if (c == '-') {
    redirect->flags = O_TRUNC;
    try(parse_bump(parse));
  }
  try(parse_blank(parse));
  return parse_word(parse, &redirect->target);
}

static bool parse_in_direct(struct parse *parse,
                            struct redirect *redirect) {
  try(parse_blank(parse));
  try(parse_word(parse, &redirect->target));
  redirect->type = redirect_path;
  redirect->flags = O_RDONLY | O_CREAT;
  return true;
}

static bool parse_in_out(struct parse *parse,
                         struct redirect *redirect) {
  try(parse_blank(parse));
  try(parse_word(parse, &redirect->target));
  redirect->type = redirect_path;
  redirect->flags = O_RDWR | O_CREAT;
  return false;
}

static bool parse_in(struct parse *parse,
                     struct redirect *redirect) {
  char c;
  try(parse_peek(parse, &c));
  switch (c) {
  case '&':
    try(parse_bump(parse));
    return parse_in_dup(parse, redirect);
    break;
  case '<':
    try(parse_bump(parse));
    return parse_in_heredoc(parse, redirect);
    break;
  case '>':
    try(parse_bump(parse));
    return parse_in_out(parse, redirect);
  default:
    return parse_in_direct(parse, redirect);
    break;
  }
}

static bool parse_out_clobber(struct parse *parse,
                              struct redirect *redirect) {
  try(parse_blank(parse));
  try(parse_word(parse, &redirect->target));
  redirect->type = redirect_path;
  redirect->flags = O_WRONLY | O_CREAT;
  return true;
}

static bool parse_out_append(struct parse *parse,
                             struct redirect *redirect) {
  try(parse_blank(parse));
  try(parse_word(parse, &redirect->target));
  redirect->type = redirect_path;
  redirect->flags = O_WRONLY | O_APPEND | O_CREAT;
  return true;
}

static bool parse_out_dup(struct parse *parse,
                          struct redirect *redirect) {
  try(parse_blank(parse));
  try(parse_word(parse, &redirect->target));
  redirect->type = redirect_descriptor;
  redirect->flags = O_WRONLY;
  return true;
}

static bool parse_out_direct(struct parse *parse,
                             struct redirect *redirect) {
  try(parse_blank(parse));
  try(parse_word(parse, &redirect->target));
  redirect->type = redirect_path;
  redirect->flags = O_WRONLY | O_EXCL | O_CREAT;
  return true;
}

static bool parse_out(struct parse *parse,
                      struct redirect *redirect) {
  char c;
  try(parse_peek(parse, &c));
  switch (c) {
  case '|':
    try(parse_bump(parse));
    return parse_out_clobber(parse, redirect);
  case '>':
    try(parse_bump(parse));
    return parse_out_append(parse, redirect);
  case '&':
    try(parse_bump(parse));
    return parse_out_dup(parse, redirect);
  default:
    return parse_out_direct(parse, redirect);
  }
}

bool parse_redirect(struct parse *parse,
                    struct redirect *redirect) {
  char c;
  try(parse_peek(parse, &c));
  switch (c) {
  case '<':
    try(parse_bump(parse));
    return parse_in(parse, redirect);
  case '>':
    try(parse_bump(parse));
    return parse_out(parse, redirect);
  default:
    return false;
  }
}

bool parse_redirect_source(struct parse *parse,
                           int *source, struct string *string) {
  char c;
  try(parse_peek(parse, &c));
  assert(isdigit(c));

  string_putc(string, c);

  unsigned int num = c - '0';
  bool overflow = false;

  try(parse_bump_peek(parse, &c));
  if (num != 0) {
    while (isdigit(c) && num <= UINT_MAX / 10) {
      if (num * 10 > UINT_MAX - (c - '0')) {
        overflow = true;
      }
      num = num * 10 + c - '0';
      try(string_putc(string, c));
      try(parse_bump_peek(parse, &c));
    }
  }

  if (c == '<' || c == '>') {
    if (overflow) {
      try(stack_push(&parse->diagnosis, &(struct diagnosis) {
        .level = diagnosis_error,
        .type = diagnosis_overflow_u,
        .location = parse->location,
        .overflow = {
          .truncated.u = num
        }
      }, sizeof(struct diagnosis)));
    }
    *source = num;
  } else {
    *source = -1;
  }
  return true;
}

bool parse_redirect_list(struct parse *parse,
                         struct stack *redirect_stack) {
  char c;
  try(parse_peek(parse, &c));
  while (isdigit(c) || c == '<' || c == '>') {
    if (isdigit(c)) {
      struct string string;
      string_init(&string);
      int source;
      try(parse_redirect_source(parse, &source, &string));
      if (source == -1) {
        string_destroy(&string);
        try(stack_push(&parse->diagnosis, &(struct diagnosis) {
          .level = diagnosis_error,
          .type = diagnosis_unexpected,
          .location = parse->location,
          .unexpect = {
            .expected = "redirect",
          },
        }, sizeof(struct diagnosis)));
        return false;
      }
      try(parse_peek(parse, &c));
      assert(c == '>' || c == '<');
    }

    struct redirect redirect;
    redirect_init(&redirect);
    try(parse_redirect(parse, &redirect));
    try(stack_push(redirect_stack, &redirect, sizeof(struct redirect)));
    try(parse_blank(parse));
    try(parse_peek(parse, &c));
  }
  return true;
}
