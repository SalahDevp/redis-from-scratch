#include "Command.h"
#include <vector>

// TODO: write result
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
