#ifndef CONNECTION_H
#define CONNECTION_H

#include <cstdint>
#include <unistd.h>

class Connection {
public:
  int fd;
  char *read_buf;
  uint32_t read_ln;
  char *write_buf;
  uint32_t write_ln;

  Connection(int fd);

  Connection(const Connection &other);

  Connection &operator=(const Connection &other);

  ~Connection();

  void allocReadBuf(uint32_t size);
  void allocWriteBuf(uint32_t size);
};

#endif