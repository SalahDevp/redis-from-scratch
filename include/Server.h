#ifndef SERVER_H
#define SERVER_H

#include "IOHandler.h"
#define PORT 8000
#define SOMAXCONN 128

class Server {
public:
  void start();
  Server(IOHandler handler) : io(handler) {};

private:
  int sd;
  IOHandler io;

  void init();
  void acceptConnections();
  void handleRequest(int reqfd);
};

#endif
