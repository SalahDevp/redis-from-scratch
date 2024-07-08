#include "Connection.h"
#include <cstdint>
#include <cstring>

Connection::Connection(int fd)
    : fd(fd), read_buf(nullptr), read_ln(0), write_buf(nullptr), write_ln(0),
      state(ConnState::REQUEST) {};

Connection::Connection(const Connection &other)
    : fd(other.fd), read_buf(nullptr), read_ln(other.read_ln),
      write_buf(nullptr), write_ln(other.write_ln), state(other.state) {
  if (other.read_buf) {
    read_buf = new char[other.read_ln];
    memcpy(read_buf, other.read_buf, other.read_ln);
  }

  if (other.write_buf) {
    write_buf = new char[other.write_ln];
    memcpy(write_buf, other.write_buf, other.write_ln);
  }
}

Connection &Connection::operator=(const Connection &other) {
  if (this != &other) {
    delete[] read_buf;
    delete[] write_buf;
    fd = other.fd;
    write_ln = other.write_ln;
    read_ln = other.read_ln;
    state = other.state;

    read_buf = new char[other.read_ln];
    memcpy(read_buf, other.read_buf, other.read_ln);
    write_buf = new char[other.write_ln];
    memcpy(write_buf, other.write_buf, other.write_ln);
  }
  return *this;
}

Connection::~Connection() {
  delete[] read_buf;
  delete[] write_buf;
}

void Connection::allocReadBuf(uint32_t size) {
  if (read_buf)
    delete[] read_buf;
  read_buf = new char[size];
  read_ln = size;
}

void Connection::allocWriteBuf(uint32_t size) {
  if (write_buf)
    delete[] write_buf;
  write_buf = new char[size];
  write_ln = size;
}
