#ifndef SERVER_H
#define SERVER_H

#include "Connection.h"
#include "IOHandler.h"
#include <list>
#include <poll.h>
#include <unordered_map>

#define PORT 8000
#define SOMAXCONN 128
#define POLL_TIMEOUT -1

class Server {
public:
  void start();
  Server(IOHandler handler) : io(handler) {};

private:
  int sd;
  IOHandler io;
  std::list<Connection *> connections;
  std::unordered_map<int, Connection *> conn_map;

  void initSocket();
  void eventLoop();
  void acceptConnections();
  void addConnection(int connfd);
  void closeConnection(int connfd);
  void handleRequest(Connection *conn);
  void sendResponse(Connection *conn);
};

#endif
