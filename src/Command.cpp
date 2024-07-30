#include "Command.h"
#include "utils.h"

void Command::execute(std::vector<std::string> &argv) {

  std::string &cmd = argv[0];
  utils::strToLowerCase(cmd);

  if (cmd == "get")
    return
}
