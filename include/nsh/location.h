#ifndef NSH_LOCATION_H
#define NSH_LOCATION_H

extern char const *program;

struct location {
  unsigned int line;
  unsigned int column;
  char const *file;
  char const *function;
};

#define locate() (struct location) {                                   \
  .line = __LINE__,                                                    \
  .file = __FILE__,                                                    \
  .function = __func__,                                                \
}

#endif // NSH_LOCATION_H
