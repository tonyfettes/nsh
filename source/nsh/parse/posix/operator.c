#include "nsh/parse/posix/operator.h"

bool is_control_operator(int ch) {
  return ch == '|' || ch == '&' || ch == ';' ||
    ch == '(' || ch == ')' || ch == '\n' || ch == '\0';
}

bool is_redirect_operator(int ch) {
  return ch == '<' || ch == '>';
}

bool is_operator(int ch) {
  return is_redirect_operator(ch) ||
         is_control_operator(ch);
}
