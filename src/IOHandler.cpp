#include "IOHandler.h"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void IOHandler::read(int fd, struct message &msg) {
  uint32_t received_ln = 0;

  // the first 4bytes are the msg length
  if (::read(fd, &msg.len, sizeof(msg.len)) != sizeof(msg.len))
    throw IOError("error reading message length");

  // allocate buffer to receive message
  msg.buf = new char[msg.len + 1];
  if (msg.buf == NULL) {
    throw IOError("error allocating buffer\n");
  }

  while (received_ln < msg.len) {
    int r = ::read(fd, msg.buf + received_ln, msg.len - received_ln);
    if (r < 0) {
      throw IOError("error reading message");
    } else if (r == 0) {
      break;
    } else {
      received_ln += r;
    }
  }
  msg.len = received_ln;
  msg.buf[msg.len] = 0;
}

void IOHandler::write(int fd, const struct message &msg) {
  // send message length
  if (::write(fd, &msg.len, sizeof(msg.len)) < 0) {
    throw IOError("error sending message");
  }

  // send message
  ssize_t bytes_sent = ::write(fd, msg.buf, (size_t)msg.len);

  if (bytes_sent < 0 || bytes_sent != (ssize_t)msg.len)
    throw IOError("error sending message");
}
