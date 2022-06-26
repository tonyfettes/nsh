#ifndef NSH_LOCATION_H
#define NSH_LOCATION_H

struct location {
  char const *file;
  unsigned int line;
  unsigned int column;
  char const *function;
};

#endif // NSH_LOCATION_H
