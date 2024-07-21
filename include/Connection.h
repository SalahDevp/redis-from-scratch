#ifndef CONNECTION_H
#define CONNECTION_H

#include "sds.h"
#include <cstdint>
#include <unistd.h>

enum class ConnState : int { REQUEST, RESPONSE };

class Connection {
public:
  int fd;
  sds query_buf;
  char **argv; // query args
  int argv_ln; // size of argv
  int argc;    // number of initialised args
  int bs_ln;   // used to track the length of current bulk string in a query
  ConnState state;

  Connection(int fd);

  ~Connection();

  Connection(const Connection &other) = delete;

  Connection &operator=(const Connection &other) = delete;
};

#endif
