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

/**
 * returns true if the full message is read
 */
void IOHandler::readQuery(Connection *conn) {

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
      conn->query_buf[sdsLen(conn->query_buf)] = '\0';
    }
  }
}

/**
 * returns true if the full message is written
 */
/* bool IOHandler::write(Connection *conn) {

  // send message length
  uint32_t msg_ln = conn->write_buf_size;

  if (conn->written_bytes == 0)
    if (::write(conn->fd, &msg_ln, sizeof(msg_ln)) < (ssize_t)sizeof(msg_ln))
{ throw IOError("error sending message");
    }

  // send message
  ssize_t bytes_sent = ::write(conn->fd, conn->write_buf +
conn->written_bytes, conn->write_buf_size - conn->written_bytes);

  if (bytes_sent <= 0)
    throw IOError("error sending message");

  conn->written_bytes += bytes_sent;

  return conn->write_buf_size == conn->written_bytes;
} */
