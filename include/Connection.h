#ifndef CONNECTION_H
#define CONNECTION_H

#include "sds.h"
#include <unistd.h>

enum class ConnState : int { REQUEST, RESPONSE };

class Connection {
public:
  int fd;
  sds query_buf;
  size_t qpos;    // used by parser
  char **argv;    // query args
  size_t argv_ln; // size of argv
  size_t argc;    // number of initialised args
  size_t bs_ln;   // used to track the length of current bulk string in a query
  ConnState state;

  Connection(int fd);

  ~Connection();

  // TODO: add a function to clear args after cmd execution to allow parsing for
  // a new cmd

  Connection(const Connection &other) = delete;

  Connection &operator=(const Connection &other) = delete;
};

#endif
