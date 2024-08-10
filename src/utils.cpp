#include "utils.h"

#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

void utils::die(const char *msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  exit(err);
}

void utils::fd_set_nb(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) {
    utils::die("fcntl error");
    return;
  }

  flags |= O_NONBLOCK;

  if (fcntl(fd, F_SETFL, flags))
    utils::die("fcntl error");
}

void utils::strToLowerCase(std::string &s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

char *utils::findChar(char *buffer, size_t buffer_size, char target) {
  for (size_t i = 0; i < buffer_size; ++i) {
    if (buffer[i] == target) {
      return buffer + i;
    }
  }
  return nullptr;
}
