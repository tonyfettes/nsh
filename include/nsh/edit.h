#ifndef NSH_EDIT_H
#define NSH_EDIT_H

#include "nsh/stack.h"
#include "nsh/string.h"
#include "nsh/terminal.h"

extern int const error_edit;

typedef unsigned int text_size_t;

enum error_edit {
  error_edit_undo_oldest,
  error_edit_redo_newest,
};

struct cursor {
  unsigned int line;
  unsigned int column;
};

enum cursor_direction {
  cursor_up,
  cursor_down,
  cursor_left,
  cursor_right,
};

void cursor_move(struct cursor *cursor, enum cursor_direction direction,
                 text_size_t size);

// Select is marked by position in buffer.
struct select {
  text_size_t left;
  text_size_t right;
};

// Range is marked by index of marks.
struct range {
  stack_size_t left;
  stack_size_t right;
};

struct buffer {
  // Stack of lines stored as `struct string`.
  struct stack line;
};

void buffer_offset(struct buffer buffer, struct cursor cursor,
                   text_size_t *offset);

void buffer_locate(struct buffer buffer, text_size_t offset,
                   struct cursor *cursor);

struct highlight {
  struct style style;
  struct range range;
};

struct edit {
  struct range remove;
  struct {
    enum {
      edit_insert_single,
      edit_insert_multiple,
    } type;
    union {
      struct string single;
      // Stack of `struct line`.
      struct stack multiple;
    };
  } insert;
};

bool edit_putc(struct edit *edit, char c);

bool edit_puts(struct edit *edit, char const *restrict source);

bool edit_putn(struct edit *edit, char const *restrict source,
               size_t size);

bool edit_push(struct edit *edit, struct string *line);

struct mark {
  enum {
    mark_left,
    mark_right,
  } direction;
};

struct mark_list {
  // Stack of `stack_size_t`. Mapping from id to index in `data`.
  struct stack index;
  // Stack of `text_size_t`. Prefix sum of distances of marks.
  struct stack fenwick;
  // Stack of `struct mark`.
  struct stack data;
};

void mark_list_init(struct mark_list *list);

void mark_list_push(struct mark_list *list, text_size_t offset,
                    struct mark mark, stack_size_t *id);

void mark_list_destroy(struct mark_list *list);

struct text {
  enum text_wrap {
    text_wrap_none,
    text_wrap_soft,
    text_wrap_hard,
  } wrap;
  unsigned int tab;
  unsigned int shift;
  // Geometry of editor might not be the same as terminal.
  struct {
    unsigned int height;
    unsigned int width;
  } geometry;
  // Cursor location by bytes.
  struct location cursor;
  struct mark_list mark_list;
  // Stack of `struct string`.
  struct stack content;
  // Stack of `struct highlight`.
  struct stack highlight;
  struct {
    struct stack edit;
    stack_size_t head;
  } history;
};

bool text_init(struct text *text);

void text_destroy(struct text *text);

bool text_edit(struct text *text, struct edit edit);

bool text_undo(struct text *text);

bool text_redo(struct text *text);

bool text_highlight(struct text *text, struct highlight highlight);

bool text_mark(struct text *text, stack_size_t *id);

struct complete_item {
  char const *type;
  char const *sort;
  char const *insert;
  char const *display;
  char const *document;
  void *data;
};

enum complete_capability {
  complete_dynamic = 1 << 0,
  complete_incremental = 1 << 1,
};

struct complete {
  unsigned int priority;
  unsigned int capability;
  struct string name;
  void (*notify)(char ch);
  bool (*query)(char const *input,
                struct complete_item const list[restrict]);
  struct complete_item const *list;
};

bool complete_init(struct complete *complete);

bool complete_update(struct complete *complete,
                     struct complete_item const *list);

bool complete_destroy(struct complete *complete);

void text_complete(struct text *text, struct complete complete);

#endif // NSH_EDIT_H
