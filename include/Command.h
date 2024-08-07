#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include "Connection.h"
#include "DataStore.h"
#include "ResponseSerializer.h"
#include <memory>
#include <stdexcept>

class Command {
public:
  Command(DataStore &ds) : ds(ds) {};
  virtual void execute(std::shared_ptr<Connection> &conn);

  class CommandError : public std::runtime_error {
  public:
    CommandError(const std::string &msg)
        : std::runtime_error("Command Error: " + msg) {}
  };

protected:
  DataStore &ds;
  ResponseSerializer serializer;
};

class SetCommand : public Command {
public:
  void execute(std::shared_ptr<Connection> &conn);
};

class GetCommand : public Command {
public:
  void execute(std::shared_ptr<Connection> &conn);
};

#endif
