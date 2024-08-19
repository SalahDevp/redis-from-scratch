#include "ResponseSerializer.h"
#include <cstring>
#include <stdexcept>
#include <string>

void ResponseSerializer::writeSimpleString(std::shared_ptr<Connection> &conn,
                                           const std::string &s) {
  bufferWrite(conn, "+");
  bufferWrite(conn, s);
  bufferWrite(conn, CRLF);
}

void ResponseSerializer::writeBulkString(std::shared_ptr<Connection> &conn,
                                         const std::string &s) {
  std::string len = std::to_string(s.length());

  bufferWrite(conn, "$");
  bufferWrite(conn, len);
  bufferWrite(conn, CRLF);
  bufferWrite(conn, s);
  bufferWrite(conn, CRLF);
}

void ResponseSerializer::writeInteger(std::shared_ptr<Connection> &conn,
                                      int i) {
  bufferWrite(conn, ":");
  bufferWrite(conn, std::to_string(i));
  bufferWrite(conn, CRLF);
}

void ResponseSerializer::writeSimpleError(std::shared_ptr<Connection> &conn,
                                          const std::string &s) {
  bufferWrite(conn, "-ERR ");
  bufferWrite(conn, s);
  bufferWrite(conn, CRLF);
}

void ResponseSerializer::bufferWrite(std::shared_ptr<Connection> &conn,
                                     const std::string &str) {
  // TODO: check full if full shift
  if (str.length() > sdsAvail(conn->res_buf)) {
    if (sdsAvail(conn->res_buf) + conn->res_pos < str.length())
      // impossible to free space
      //  TODO: increase buffer size
      throw std::runtime_error(
          "impossible to free up space in response buffer");

    // remove the part of the buffer that is already sent to free space
    sdsShiftL(conn->res_buf, conn->res_pos);
  }

  sdsCat(conn->res_buf, str.c_str(), str.length());
}
