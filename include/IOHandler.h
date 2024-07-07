#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <cstring>
#include <stdexcept>
#include <unistd.h>

struct message {
  char *buf;
  uint32_t len;

  message() : buf(nullptr), len(0) {}

  message(const message &other) : len(other.len) {
    buf = new char[len];
    std::memcpy(buf, other.buf, len);
  }

  // Copy assignment operator
  message &operator=(const message &other) {
    if (this != &other) {
      delete[] buf; // Free existing resource
      len = other.len;
      buf = new char[len];
      std::memcpy(buf, other.buf, len);
    }
    return *this;
  }

  // Destructor
  ~message() {
    if (buf)
      delete[] buf;
  }
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
