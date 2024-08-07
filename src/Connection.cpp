#include "Connection.h"
#include <cstring>

Connection::Connection(int fd)
    : fd(fd), qpos(0), argc(0), bs_ln(0), res_pos(0),
      state(ConnState::REQUEST) {
  query_buf = sdsAlloc(PROTO_IOBUF_LEN);
  res_buf = sdsAlloc(PROTO_IOBUF_LEN);
}

Connection::~Connection() {
  if (query_buf) {
    sdsFree(query_buf);
  }
  if (res_buf)
    sdsFree(res_buf);
}
