#ifndef IOHANDLER_H
#define IOHANDLER_H

#include "Connection.h"
#include <cstring>
#include <memory>
#include <stdexcept>
#include <unistd.h>

class IOHandler {
public:
  void readQuery(std::shared_ptr<Connection> &conn);
  void write(std::shared_ptr<Connection> &conn);

  class IOError : public std::runtime_error {
  public:
    IOError(const char *msg) : std::runtime_error(msg) {}
  };
};

#endif
