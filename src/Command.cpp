#include "Command.h"
#include <vector>

std::unique_ptr<Command>
CommandFactory::getCommand(const std::vector<std::string> &argv, DataStore &ds,
                           ResponseSerializer &serializer) {
  std::string cmd = argv[0];
  utils::strToLowerCase(cmd);

  if (cmd == "get")
    return std::make_unique<GetCommand>(ds, serializer);

  if (cmd == "set")
    return std::make_unique<SetCommand>(ds, serializer);

  if (cmd == "del")
    return std::make_unique<DelCommand>(ds, serializer);

  throw Command::CommandError("command '" + cmd + "' is invalid.");
}

void SetCommand::execute(std::shared_ptr<Connection> &conn) {
  if (conn->argv.size() != 3)
    throw CommandError("wrong number of arguments for 'set' command.");

  ds.set(conn->argv[1], conn->argv[2]);
  serializer.writeSimpleString(conn, "OK");
}

void GetCommand::execute(std::shared_ptr<Connection> &conn) {
  if (conn->argv.size() != 2)
    throw CommandError("wrong number of arguments for 'get' command.");

  const std::string &value = ds.get(conn->argv[1]);
  serializer.writeBulkString(conn, value);
}

void DelCommand::execute(std::shared_ptr<Connection> &conn) {
  if (conn->argv.size() < 2)
    throw CommandError("wrong number of arguments for 'del' command");

  int c = 0;
  for (size_t i = 1; i < conn->argv.size(); i++) {
    if (ds.del(conn->argv[i]))
      c++;
  }

  serializer.writeInteger(conn, c);
}
