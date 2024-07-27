#include "Server.h"
#include "utils.h"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netinet/ip.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void Server::start() {
  initSocket();

  eventLoop();
}

void Server::initSocket() {
  sd = socket(PF_INET, SOCK_STREAM, 0);
  int val = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
  // set async mode
  utils::fd_set_nb(sd);

  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(0); // listen on 0.0.0.0

  if (bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    utils::die("error binding to address");
  }

  if (listen(sd, SOMAXCONN)) {
    utils::die("error listening to address");
  }
  std::cout << "Server listening on port: " << PORT << std::endl;
}

void Server::eventLoop() {
  std::vector<struct pollfd> poll_fds;

  // event loop
  while (true) {
    poll_fds.clear();
    struct pollfd sd_poll = {sd, POLLIN, 0};
    poll_fds.push_back(sd_poll);

    for (Connection *conn : connections) {
      poll_fds.push_back(
          {conn->fd,
           (conn->state == ConnState::REQUEST) ? (short)POLLIN : (short)POLLOUT,
           0});
    }

    int pv = poll(poll_fds.data(), poll_fds.size(), POLL_TIMEOUT);

    if (pv < 0) {
      utils::die("error in poll");
    } else if (pv == 0)
      continue;

    // check ready sockets
    for (auto pfd : poll_fds) {
      if (pfd.revents == 0)
        continue;
      else if ((pfd.revents & (POLLIN | POLLOUT)) != 0) {
        if (pfd.fd == sd) {
          acceptConnections();
        } else if (conn_map[pfd.fd]->state == ConnState::REQUEST) {
          handleRequest(conn_map[pfd.fd]);
        } /* else if (conn_map[pfd.fd]->state == ConnState::RESPONSE) {
          sendResponse(conn_map[pfd.fd]);
        } */
      } else { // error or client closed connection
        if (pfd.fd == sd) {
          utils::die("error in poll results");
        } else {
          closeConnection(pfd.fd);
        }
      }
    }
  }
}

void Server::acceptConnections() {
  while (true) {
    struct sockaddr_in client_addr = {};
    socklen_t addrlen = sizeof(client_addr);
    int connfd = accept(sd, (struct sockaddr *)&client_addr, &addrlen);
    if (connfd < 0) {
      if (errno == EWOULDBLOCK)
        return; // no pending connections
      utils::die("error in accept");
    }
    addConnection(connfd);
  }
}

void Server::addConnection(int connfd) {
  Connection *newConn = new Connection(connfd);

  connections.push_back(newConn);
  conn_map[connfd] = newConn;
}

void Server::closeConnection(int connfd) {
  auto it = conn_map.find(connfd);
  if (it != conn_map.end()) {
    delete it->second;
    connections.remove(it->second);
    conn_map.erase(it);
    close(connfd);
  }
}

void Server::handleRequest(Connection *conn) {
  try {
    io.readQuery(conn);
    /* if the available space in the buffer is less than the threshold remove
the already parsed portion of the buffer to free up space for the next read.*/
    if ((double)sdsAvail(conn->query_buf) / (double)PROTO_IOBUF_LEN < 0.25) {
      sdsShiftL(conn->query_buf, conn->qpos);
    }
    while (parser.parseQuery(conn)) {
      // TODO: execute command
    }

  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    closeConnection(conn->fd);
  }
}

/* void Server::sendResponse(Connection *conn) {
  try {
    bool isWriteDone = io.write(conn);
    if (isWriteDone) {
      closeConnection(conn->fd);
    }
    catch (const IOHandler::IOError &e) {
      std::cerr << e.what() << std::endl;
      closeConnection(conn->fd);
    }
  } */

int main() {
  Server server;
  server.start();

  return 0;
}
