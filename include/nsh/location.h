#ifndef NSH_LOCATION_H
#define NSH_LOCATION_H

extern char const *program;

struct location {
  char const *file;
  unsigned int line;
  unsigned int column;
  char const *function;
};

#define location() (struct location) {                                 \
  .file = __FILE__,                                                    \
  .line = __LINE__,                                                    \
  .function = __func__,                                                \
}

#endif // NSH_LOCATION_H
