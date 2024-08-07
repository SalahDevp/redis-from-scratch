#ifndef RESPONSE_SERIALIZER_H
#define RESPONSE_SERIALIZER_H

#include "Connection.h"
#include <memory>
#include <string>

#define CRLF "\r\n"

class ResponseSerializer {
public:
  void writeSimpleString(std::shared_ptr<Connection> &conn,
                         const std::string &s);

  void writeBulkString(std::shared_ptr<Connection> &conn, const std::string &s);

private:
  void bufferWrite(std::shared_ptr<Connection> &conn, const std::string &str);
};
#endif
