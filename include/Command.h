#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include "Connection.h"
#include "DataStore.h"
#include "ResponseSerializer.h"
#include "utils.h"
#include <memory>
#include <stdexcept>
#include <string>

class Command {
public:
  Command(DataStore &ds, ResponseSerializer &serializer)
      : ds(ds), serializer(serializer) {};
  virtual void execute(std::shared_ptr<Connection> &conn) = 0;

  class CommandError : public std::runtime_error {
  public:
    CommandError(const std::string &msg)
        : std::runtime_error("Command Error: " + msg) {}
  };

  virtual ~Command() = default;

protected:
  DataStore &ds;
  ResponseSerializer serializer;
};

class SetCommand : public Command {
public:
  SetCommand(DataStore &ds, ResponseSerializer &serializer)
      : Command(ds, serializer) {};
  void execute(std::shared_ptr<Connection> &conn);
};

class GetCommand : public Command {
public:
  GetCommand(DataStore &ds, ResponseSerializer &serializer)
      : Command(ds, serializer) {};
  void execute(std::shared_ptr<Connection> &conn);
};

class DelCommand : public Command {
public:
  DelCommand(DataStore &ds, ResponseSerializer &serializer)
      : Command(ds, serializer) {};
  void execute(std::shared_ptr<Connection> &conn);
};

class CommandFactory {
public:
  static std::unique_ptr<Command>
  getCommand(const std::vector<std::string> &argv, DataStore &ds,
             ResponseSerializer &serializer);
};

#endif
