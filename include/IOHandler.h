#ifndef IOHANDLER_H
#define IOHANDLER_H

#include "Connection.h"
#include <cstring>
#include <stdexcept>
#include <unistd.h>

#define PROTO_IOBUF_LEN 16 * 1024

class IOHandler {
public:
  void readQuery(Connection *conn);
  bool write(Connection *conn);

  class IOError : public std::runtime_error {
  public:
    IOError(const char *msg) : std::runtime_error(msg) {}
  };
};

#endif
