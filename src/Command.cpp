#include "Command.h"
#include <vector>

// TODO: write result
void SetCommand::execute(const std::vector<std::string> &argv) {
  if (argv.size() == 3)
    throw CommandError("wrong number of arguments for 'set' command.");

  ds.set(argv[1], argv[2]);
}

void GetCommand::execute(const std::vector<std::string> &argv) {
  if (argv.size() != 2)
    throw CommandError("wrong number of arguments for 'get' command.");

  ds.get(argv[1]);
}
