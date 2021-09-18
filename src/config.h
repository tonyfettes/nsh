#ifndef CONFIG_H
#define CONFIG_H

#define LINE_SIZE 1024

typedef struct {
  char *prompt;
} config;

config *config_from(const char *filename);
void config_free(config *victim);

#endif
