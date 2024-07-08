#include "Server.h"
#include "utils.h"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void Server::start() {
  initSocket();

  acceptConnections();
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

void Server::acceptConnections() {
  while (true) {
    struct sockaddr_in client_addr = {};
    socklen_t addrlen = sizeof(client_addr);
    int connfd = accept(sd, (struct sockaddr *)&client_addr, &addrlen);
    if (connfd < 0) {
      continue; // error
    }

    handleRequest(connfd);
    close(connfd);
  }
}

void Server::handleRequest(int reqfd) {
  struct message msg;
  try {
    io.read(reqfd, msg);
    printf("received message: %s\n", msg.buf);
  } catch (const IOHandler::IOError &e) {
    std::cerr << e.what() << std::endl;
  }
}

int main() {
  IOHandler ioHandler;
  Server server(ioHandler);
  server.start();

  return 0;
}
