#include "IOHandler.h"
#include "sds.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void IOHandler::readQuery(std::shared_ptr<Connection> &conn) {

  if (!conn->query_buf) {
    conn->query_buf = sdsAlloc(PROTO_IOBUF_LEN);
  }

  while (true) {
    int r = read(conn->fd, conn->query_buf + sdsLen(conn->query_buf),
                 sdsAvail(conn->query_buf));
    if (r <= 0) {
      if (r == 0) { // EOF
        throw IOError("connection closed");
      } else if (errno != EWOULDBLOCK)
        throw IOError("error reading message");
      else
        break;
    } else {
      sdsSetLen(conn->query_buf, sdsLen(conn->query_buf) + r);
      // NOTE: this can be removed
      conn->query_buf[sdsLen(conn->query_buf)] = '\0';
    }
  }
}

void IOHandler::write(std::shared_ptr<Connection> &conn) {

  while (true) {
    int w = ::write(conn->fd, conn->res_buf + conn->res_pos,
                    sdsLen(conn->res_buf) - conn->res_pos);
    if (w <= 0) {
      if (w == 0) { // EOF
        throw IOError("connection closed");
      } else if (errno != EWOULDBLOCK)
        throw IOError("error reading message");
      else
        break;
    } else {
      conn->res_pos += w;
      if(conn->res_pos >= sdsLen(conn->res_buf)) {
        break;
      }
    }
  }
}
