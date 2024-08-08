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
  SetCommand(DataStore &ds) : Command(ds) {};
  void execute(std::shared_ptr<Connection> &conn);
};

class GetCommand : public Command {
public:
  GetCommand(DataStore &ds) : Command(ds) {};
  void execute(std::shared_ptr<Connection> &conn);
};

class CommandFactory {
public:
  static std::unique_ptr<Command>
  getCommand(const std::vector<std::string> &argv, DataStore &ds) {
    std::string cmd = argv[0];
    utils::strToLowerCase(cmd);

    if (cmd == "get")
      return std::make_unique<GetCommand>(ds);

    if (cmd == "set")
      return std::make_unique<SetCommand>(ds);

    throw std::runtime_error("command is invalid.");
  }
};

#endif
