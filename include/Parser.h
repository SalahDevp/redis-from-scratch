#ifndef PARSER_H
#define PARSER_H

#include "Connection.h"
#include <cstring>
#include <stdexcept>
#include <string>
class Parser {
public:
  /**
   * returns true if a full command is parsed
   */
  bool parseQuery(Connection *conn);

  class ParserError : public std::runtime_error {
  public:
    ParserError(const char *msg)
        : std::runtime_error("Parser Error: " + std::string(msg)) {}
  };

private:
  /**
   * return size if the size part is fully loaded in buffer `s`
   * (*<decimal-str>\r\n) , -1 otherwise. throws ParserError if the size string
   * is invalid
   */
  size_t parseSize(sds s, size_t *pos);
};
#endif
