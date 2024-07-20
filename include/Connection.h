#ifndef CONNECTION_H
#define CONNECTION_H

#include <cstdint>
#include <unistd.h>

enum class ConnState : int { REQUEST, RESPONSE };

class Connection {
public:
  int fd;
  char *read_buf;
  uint32_t read_buf_size;
  uint32_t read_bytes; // bytes read so far
  char *write_buf;
  uint32_t write_buf_size;
  uint32_t written_bytes; // bytes written so far
  ConnState state;
  uint16_t res_status;

  Connection(int fd);

  Connection(const Connection &other);

  Connection &operator=(const Connection &other);

  ~Connection();

  void allocReadBuf(uint32_t msg_len);
  void allocWriteBuf(uint32_t msg_len);
};

#endif
