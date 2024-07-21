#include "Connection.h"
#include <cstring>

Connection::Connection(int fd)
    : fd(fd), query_buf(nullptr), argv(nullptr), argv_ln(0), argc(0), bs_ln(0),
      state(ConnState::REQUEST) {}

Connection::~Connection() {
  if (query_buf) {
    freeSds(query_buf);
  }

  for (int i = 0; i < argc; ++i) {
    delete[] argv[i];
  }
  delete[] argv;
}
