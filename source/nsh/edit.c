#include "nsh/edit.h"

#include <termios.h>
#include <unistd.h>

int const error_edit = 0;

bool highlight_blend(struct stack highlight, struct stack *target) {
  for (struct highlight *hl = stack_head(&highlight, 0);
       hl != stack_head(&highlight, 0); hl++) {
  }
}

bool edit_putc(struct edit *edit, char c) {
  switch (edit->insert.type) {
  case edit_insert_single:
    return string_putc(&edit->insert.single, c);
    break;
  case edit_insert_multiple:
    if (edit->insert.multiple.size == 0) {
      if (!stack_alloc(&edit->insert.multiple, sizeof(struct string))) {
        return false;
      }
    }
    struct string *line = stack_tail(&edit->insert.multiple, 0);
    string_init(line);
    return string_putc(line, c);
    break;
  }
}

bool edit_puts(struct edit *edit, char const *restrict source) {
  switch (edit->insert.type) {
  case edit_insert_single:
    return string_puts(&edit->insert.single, source);
    break;
  case edit_insert_multiple:
    if (edit->insert.multiple.size == 0) {
      if (!stack_alloc(&edit->insert.multiple, sizeof(struct string))) {
        return false;
      }
    }
    struct string *line = stack_tail(&edit->insert.multiple, 0);
    string_init(line);
    return string_puts(line, source);
    break;
  }
}

bool edit_putn(struct edit *edit, char const *restrict source,
               size_t size) {
  switch (edit->insert.type) {
  case edit_insert_single:
    return string_putn(&edit->insert.single, source, size);
    break;
  case edit_insert_multiple:
    if (edit->insert.multiple.size == 0) {
      if (!stack_alloc(&edit->insert.multiple, sizeof(struct string))) {
        return false;
      }
    }
    struct string *line = stack_tail(&edit->insert.multiple, 0);
    string_init(line);
    return string_putn(line, source, size);
    break;
  }
}

bool edit_push(struct edit *edit, struct string *line) {
  if (edit->insert.type == edit_insert_single) {
    struct string temp;
    temp = edit->insert.single;
    stack_init(&edit->insert.multiple);
    if (!stack_push(&edit->insert.multiple, &temp, sizeof(temp))) {
      edit->insert.single = temp;
      return false;
    }
    edit->insert.type = edit_insert_multiple;
  }
  if (!stack_alloc(&edit->insert.multiple, sizeof(struct string))) {
    return false;
  }
  struct string *new = stack_tail(&edit->insert.multiple, 0);
  *new = *line;
  string_init(line);
  assert(stack_bump(&edit->insert.multiple, sizeof(struct string)));
  return true;
}

bool text_init(struct text *text) {
  text->cursor.column = 0;
  text->cursor.line = 0;
  stack_init(&text->history.edit);
  text->history.head = 0;
  stack_init(&text->content);
  return true;
}

bool text_putc(struct text *text, char ch) {
  if (text->history.head < text->history.edit.size) {
    stack_resize(&text->history.edit, text->history.head);
  }
  struct string *line;
  line = stack_head(&text->content,
                    text->cursor.line * sizeof(struct string));
  if (!stack_alloc(&text->history.edit, sizeof(struct edit))) {
    return false;
  }
  if (!string_intc(line, text->cursor.column, ch)) {
    return false;
  }
  struct edit edit = {
    .remove = {
      .left = text->cursor,
      .right = text->cursor,
    },
    .insert.type = edit_insert_single,
  };
  string_init(&edit.insert.single);
  text->cursor.column++;
  assert(stack_push(&text->history.edit, &edit, sizeof(edit)));
  text->history.head = text->history.edit.size;
  return true;
}

bool text_puts(struct text *text, char const *restrict s) {
  if (text->history.head < text->history.edit.size) {
    stack_resize(&text->history.edit, text->history.head);
  }
  struct string *line;
  line = stack_head(&text->content,
                    text->cursor.line * sizeof(struct string));
  if (!stack_alloc(&text->history.edit, sizeof(struct edit))) {
    return false;
  }
  if (!string_ints(line, text->cursor.column, s)) {
    return false;
  }
  struct edit edit = {
    .remove = {
      .left = text->cursor,
      .right = {
        .column = text->cursor.column + strlen(s),
        .line = text->cursor.line,
      },
    },
    .insert.type = edit_insert_single,
  };
  string_init(&edit.insert.single);
  assert(stack_push(&text->history.edit, &edit, sizeof(edit)));
  text->history.head = text->history.edit.size;
  return true;
}

// bool text_undo(struct text *text) {
//   if (text->history.edit.size == 0) {
//     throw(text, undo_oldest);
//     return false;
//   }
//   struct edit *edit;
//   edit = stack_head(&text->history.edit,
//                     text->history.head - sizeof(struct edit));
//   struct string *line;
//   line = stack_head(&text->content,
//                     edit->position.line * sizeof(struct string));
//   struct string temp;
//   string_init(&temp);
//   if (!string_append(&temp, *line, edit->position.column,
//                      edit->size)) {
//     string_destroy(&temp);
//     return false;
//   }
//   assert(string_remove(line, edit->position.column, edit->size));
//   edit->size = edit->content.size;
//   assert(string_insert(line, edit->position.column, edit->content));
//   string_swap(&edit->content, &temp);
//   string_destroy(&temp);
//   text->history.head -= sizeof(struct edit);
//   return true;
// }
//
// bool text_redo(struct text *text) {
//   if (text->history.head == text->history.edit.size) {
//     throw(text, redo_newest);
//     return false;
//   }
//   struct edit *edit;
//   edit = stack_head(&text->history.edit, text->history.head);
//   struct string *line;
//   line = stack_head(&text->content,
//                     edit->position.line * sizeof(struct string));
//   struct string temp;
//   string_init(&temp);
//   if (!string_append(&temp, *line, edit->position.column, edit->size)) {
//     string_destroy(&temp);
//     return false;
//   }
//   assert(string_remove(line, edit->position.column, edit->size));
//   edit->size = edit->content.size;
//   assert(string_insert(line, edit->position.column, edit->content));
//   string_swap(&edit->content, &temp);
//   string_destroy(&temp);
//   text->history.head += sizeof(struct edit);
//   return true;
// }

// void text_destroy(struct text *text) {
//   for (struct edit *edit = stack_head(&text->history.edit, 0);
//        edit != stack_tail(&text->history.edit, 0); edit++) {
//     string_destroy(&edit->content);
//   }
//   stack_destroy(&text->history.edit);
//   for (struct string *line = stack_head(&text->content, 0);
//        line != stack_tail(&text->content, 0); line++) {
//     string_destroy(line);
//   }
//   stack_destroy(&text->content);
// }
