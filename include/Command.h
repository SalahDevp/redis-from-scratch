#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include "DataStore.h"
#include <string>
#include <vector>

class Command {
public:
  Command(DataStore &ds) : ds(ds) {};
  virtual void execute(Connection);

private:
  DataStore &ds;
};

#endif
