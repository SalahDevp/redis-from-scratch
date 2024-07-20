#include "IOHandler.h"
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

/**
 * returns true if the full message is read
 */
bool IOHandler::read(Connection *conn) {

  if (conn->read_buf_size == 0) {
    // the first 4bytes are the msg_lngth
    uint32_t msg_ln;
    if (::read(conn->fd, &msg_ln, sizeof(msg_ln)) != sizeof(msg_ln))
      throw IOError("error reading message length");

    conn->allocReadBuf(msg_ln);
  }

  int r = ::read(conn->fd, conn->read_buf + conn->read_bytes,
                 conn->read_buf_size - conn->read_bytes);
  if (r <= 0) {
    throw IOError("error reading message");
  } else {
    conn->read_bytes += r;
  }

  conn->read_buf[conn->read_bytes] = '\0';

  return conn->read_bytes == conn->read_buf_size;
}

/**
 * returns true if the full message is written
 */
bool IOHandler::write(Connection *conn) {

  // send message length
  uint32_t msg_ln = conn->write_buf_size;

  if (conn->written_bytes == 0)
    if (::write(conn->fd, &msg_ln, sizeof(msg_ln)) < (ssize_t)sizeof(msg_ln)) {
      throw IOError("error sending message");
    }

  // send message
  ssize_t bytes_sent = ::write(conn->fd, conn->write_buf + conn->written_bytes,
                               conn->write_buf_size - conn->written_bytes);

  if (bytes_sent <= 0)
    throw IOError("error sending message");

  conn->written_bytes += bytes_sent;

  return conn->write_buf_size == conn->written_bytes;
}
