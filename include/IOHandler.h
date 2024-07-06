#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <stdexcept>
#include <unistd.h>

struct message {
  char *buf;
  uint32_t len;

  message() : buf(nullptr), len(0) {}
  ~message() { delete[] buf; }
};

class IOHandler {
public:
  void read(int fd, struct message &msg);
  void write(int fd, const struct message &msg);

  class IOError : public std::runtime_error {
  public:
    IOError(const char *msg) : std::runtime_error(msg) {}
  };
};

#endif
