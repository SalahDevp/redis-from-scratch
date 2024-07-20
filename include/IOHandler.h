#ifndef IOHANDLER_H
#define IOHANDLER_H

#include "Connection.h"
#include <cstring>
#include <stdexcept>
#include <unistd.h>

class IOHandler {
public:
  bool read(Connection *conn);
  bool write(Connection *conn);

  class IOError : public std::runtime_error {
  public:
    IOError(const char *msg) : std::runtime_error(msg) {}
  };
};

#endif
