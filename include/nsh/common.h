#ifndef NSH_COMMON_H
#define NSH_COMMON_H

#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdnoreturn.h>

#include "nsh/log.h"
#include "nsh/error.h"
#include "nsh/memory.h"
#include "nsh/assert.h"

void ignore(void *none, ...);

#endif // NSH_COMMON_H
