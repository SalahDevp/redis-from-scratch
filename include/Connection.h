#ifndef CONNECTION_H
#define CONNECTION_H

#include "sds.h"
#include <string>
#include <unistd.h>
#include <vector>

#define PROTO_IOBUF_LEN 16 * 1024

enum class ConnState : int { REQUEST, RESPONSE };

class Connection {
public:
  int fd;
  sds query_buf;
  size_t qpos; // used by parser to track current parsing position
  std::vector<std::string> argv; // query args
  size_t argc;                   // total number of args in argv
  size_t bs_ln; // used by parser to track the length of current bulk string in
                // a query
  sds res_buf;
  size_t res_pos; // to track sent parts of the res_buf
  ConnState state;

  Connection(int fd);

  ~Connection();

  Connection(const Connection &other) = delete;

  Connection &operator=(const Connection &other) = delete;

  void clearArgs();
};

#endif
