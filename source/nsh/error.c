#include "nsh/error.h"

struct error error;

const int error_system = 0;

void throw_system(int error_code) {
  error.category = &error_system;
  error.code = error_code;
}
