#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include "DataStore.h"
#include <stdexcept>
#include <vector>

class Command {
public:
  Command(DataStore &ds) : ds(ds) {};
  virtual void execute(const std::vector<std::string> &argv);

  class CommandError : public std::runtime_error {
  public:
    CommandError(const std::string &msg)
        : std::runtime_error("Command Error: " + msg) {}
  };

protected:
  DataStore &ds;
};

class SetCommand : public Command {
public:
  void execute(const std::vector<std::string> &argv);
};

class GetCommand : public Command {
public:
  void execute(const std::vector<std::string> &argv);
};

#endif
