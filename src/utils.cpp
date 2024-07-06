#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void die(const char *msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  exit(err);
}
