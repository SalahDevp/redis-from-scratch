#ifndef SERVER_H
#define SERVER_H

#include "Connection.h"
#include "IOHandler.h"
#include "Parser.h"
#include <cerrno>
#include <list>
#include <memory>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <unordered_map>

#define PORT 8000
#define SOMAXCONN 128
#define POLL_TIMEOUT -1
#define BUF_AVAIL_THRESHOLD 0.25

class Server {
public:
  void start();
  Server() : io(), parser() {};
  Server(IOHandler handler) : io(handler) {};

  class ServerError : public std::runtime_error {
  public:
    ServerError(const std::string &msg)
        : std::runtime_error("Server Error: " + msg + " [errno: ]" +
                             std::to_string(errno)) {};
  };

  class ClientError : public std::runtime_error {
  public:
    ClientError(const std::string &msg)
        : std::runtime_error("Client Error: " + msg) {};
  };

private:
  int sd;
  IOHandler io;
  Parser parser;
  std::list<std::shared_ptr<Connection>> connections;
  std::unordered_map<int, std::shared_ptr<Connection>> conn_map;

  void initSocket();
  void eventLoop();
  void acceptConnections();
  void addConnection(int connfd);
  void closeConnection(int connfd);
  void handleRequest(std::shared_ptr<Connection> &conn);
  void sendResponse(std::shared_ptr<Connection> &conn);
};

#endif
