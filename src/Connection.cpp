#include "Connection.h"
#include <cstring>

Connection::Connection(int fd)
    : fd(fd), query_buf(nullptr), qpos(0), argv(nullptr), argv_ln(0), argc(0),
      bs_ln(0), state(ConnState::REQUEST) {}

Connection::~Connection() {
  if (query_buf) {
    sdsFree(query_buf);
  }

  for (size_t i = 0; i < argc; ++i) {
    delete[] argv[i];
  }
  delete[] argv;
}
