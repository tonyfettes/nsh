#include "nsh/parse/operator.h"

bool is_control_operator(int ch) {
  return ch == '|' || ch == '&' || ch == ';' ||
    ch == '(' || ch == ')' || ch == '\n' || ch == '\0';
}

bool is_operator(int ch) {
  return ch == '<' || ch == '>' || is_control_operator(ch);
}
