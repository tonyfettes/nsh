#include "nsh/builtin.h"
#include "nsh/context.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

static char const *const doc =
"Usage: %s [-L|-P] [directory]\n"
"   or: %s -\n"
"\n"
"Options:\n"
"\t-L\tHandle the operand dot-dot logically; symbolic link components "
      "shall not be resolved before dot-dot components are processed.\n"
"\t-P\tHandle the operand dot-dot physically; symbolic link components "
      "shall be resolved before dot-dot components are processed.\n"
"\n"
"For more details see cd(1p)\n";

struct target {
  char const *path;
  int flags;
};

static void builtin_cd_fatal(char const *restrict program,
                             char const *restrict message) {
  fprintf(stderr, "%s%s%s.\n", program, program[0] ? ": " : "",
          message);
}

static bool builtin_cd_cdpath(char const *restrict program,
                              struct string *directory) {
  char const *path_list = getenv("CDPATH");
  if (!path_list) {
    return false;
  }
  struct string path;
  string_init(&path);
  struct stat rstat;
  while (true) {
    size_t len = strchr(path_list, ':') - path_list;
    if  (len == 0) {
      string_puts(&path, "./");
    } else {
      string_putn(&path, path_list, len);
      if (string_data(&path)[len - 1] != '/') {
        string_putc(&path, '/');
      }
    }
    string_concat(&path, *directory);
    if (stat(string_data(&path), &rstat) == -1) {
      builtin_cd_fatal(program, strerror(errno));
      return EXIT_FAILURE;
    }
    if (S_ISDIR(rstat.st_mode)) {
      string_swap(directory, &path);
      string_clear(&path);
    }
    string_clear(&path);
    path_list += len + 1;
  }
  string_destroy(&path);
}

int builtin_cd(int argc, char *const argv[]) {
  if (argc > 3) {
    builtin_cd_fatal(argv[0], strerror(E2BIG));
    return EXIT_FAILURE;
  }

  struct string directory;
  string_init(&directory);
  if (argc == 1) {
    char const *home = getenv("HOME");
    if (strlen(home) == 0) {
      builtin_cd_fatal(argv[0], "Empty HOME environment variable");
      return EXIT_FAILURE;
    }
    string_refer(&directory, home);
  } else {
    string_refer(&directory, argv[1]);
  }

  enum {
    builtin_cd_none,
    builtin_cd_logical,
    builtin_cd_physical,
  } type;

  // if (argv[1][0] == '-') {
  //   switch (argv[1][1]) {
  //   case 'L':
  //     todo("Hanlding operator `..' logically");
  //     break;
  //   case 'P':
  //     todo("Hanlding operator `..' physically");
  //     break;
  //   case 'h':
  //     fprintf(stderr, doc, argv[0], argv[0]);
  //     break;
  //   case '\0':
  //     context.pwd
  //     break;
  //   }
  // } else {
  //   directory = argv[1];
  //   if (argc > 2) {
  //     fprintf(stderr, "%s%s%s.\n", argv[0], argv[0] ? ": " : "",
  //             strerror(E2BIG));
  //     return EXIT_FAILURE;
  //   }
  // }

  char const *dirs = string_data(&directory);
  if (dirs[0] == '/') {
    if (chdir(dirs) == -1) {
      builtin_cd_fatal(argv[0], strerror(errno));
      return EXIT_FAILURE;
    }
  } else if (dirs[0] == '.' &&
             (dirs[1] == '/' ||
              (dirs[1] == '.' && dirs[2] == '/'))) {
  } else {
    if (!builtin_cd_cdpath(argv[0], &directory)) {
      builtin_cd_fatal(argv[0], strerror(errno));
      return EXIT_FAILURE;
    }
  }

  if (type == builtin_cd_physical) {
  }
  if (chdir(string_data(&directory)) == -1) {
    builtin_cd_fatal(argv[0], strerror(errno));
    return EXIT_FAILURE;
  }
}
