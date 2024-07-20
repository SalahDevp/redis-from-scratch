#include "Connection.h"
#include <cstdint>
#include <cstring>

Connection::Connection(int fd)
    : fd(fd), read_buf(nullptr), read_buf_size(0), read_bytes(0),
      write_buf(nullptr), write_buf_size(0), written_bytes(0), res_status(0),
      state(ConnState::REQUEST) {}

Connection::Connection(const Connection &other)
    : fd(other.fd), read_buf(nullptr), read_buf_size(other.read_buf_size),
      read_bytes(other.read_bytes), write_buf(nullptr),
      write_buf_size(other.write_buf_size), written_bytes(other.written_bytes),
      res_status(other.res_status), state(other.state) {
  if (other.read_buf) {
    read_buf = new char[other.read_buf_size + 1];
    std::memcpy(read_buf, other.read_buf, other.read_buf_size + 1);
  }

  if (other.write_buf) {
    write_buf = new char[other.write_buf_size + 1];
    std::memcpy(write_buf, other.write_buf, other.write_buf_size + 1);
  }
}

Connection &Connection::operator=(const Connection &other) {
  if (this != &other) {
    delete[] read_buf;
    delete[] write_buf;

    fd = other.fd;
    write_buf_size = other.write_buf_size;
    read_buf_size = other.read_buf_size;
    read_bytes = other.read_bytes;
    written_bytes = other.written_bytes;
    res_status = other.res_status;
    state = other.state;

    if (other.read_buf) {
      read_buf = new char[other.read_buf_size + 1];
      std::memcpy(read_buf, other.read_buf, other.read_buf_size + 1);
    } else {
      read_buf = nullptr;
    }

    if (other.write_buf) {
      write_buf = new char[other.write_buf_size + 1];
      std::memcpy(write_buf, other.write_buf, other.write_buf_size + 1);
    } else {
      write_buf = nullptr;
    }
  }
  return *this;
}

Connection::~Connection() {
  delete[] read_buf;
  delete[] write_buf;
}
/**
 * allocates memory of size msg_len+1 and inits read_buf_size
 */
void Connection::allocReadBuf(uint32_t msg_len) {
  if (read_buf)
    delete[] read_buf;
  read_buf = new char[msg_len + 1];
  read_buf[0] = '\0';
  read_buf_size = msg_len;
}

/**
 * allocates memory of size msg_len+1 and inits write_buf_size
 */
void Connection::allocWriteBuf(uint32_t msg_len) {
  if (write_buf)
    delete[] write_buf;
  write_buf = new char[msg_len + 1];
  write_buf[0] = '\0';
  write_buf_size = msg_len;
}
