#include "Parser.h"
#include "Connection.h"
#include <cstring>

bool Parser::parseQuery(Connection *conn) {
  if (!conn->argv_ln) { // read array length
    if (conn->query_buf[conn->qpos] != '*')
      throw Parser::ParserError("missing '*'");

    conn->qpos++;

    size_t arrlen = parseSize(conn->query_buf, &conn->qpos);
    if (arrlen < 0) {
      conn->qpos--;
      return false;
    }

    conn->argv_ln = arrlen;
    conn->argv = new char *[arrlen];
    conn->argc = 0;
  }

  while (conn->argc < conn->argv_ln) {
    if (!conn->bs_ln) { // read bulk string length

      if (conn->query_buf[conn->qpos] != '$')
        throw Parser::ParserError("missing '$'");

      conn->qpos++;

      size_t size = parseSize(conn->query_buf, &conn->qpos);
      if (size < 0) {
        conn->qpos--;
        return false;
      }

      conn->bs_ln = size;
      conn->argv[conn->argc] = new char[size + 1];
    }

    // read bulk string
    if (sdsLen(conn->query_buf) - conn->qpos < conn->bs_ln + 2)
      return false;

    memcpy(conn->argv[conn->argc], conn->query_buf + conn->qpos, conn->bs_ln);
    conn->argv[conn->argc][conn->bs_ln] = '\0';

    conn->argc++;
    conn->bs_ln = 0;
  }

  return true;
}

size_t Parser::parseSize(sds s, size_t *pos) {

  char *cr = strchr(s + *pos, '\r');
  if (!cr || (sdsLen(s) == (size_t)(cr - s) + 1) || *(cr + 1) != '\n')
    return -1; // no CRLF in query

  *cr = '\0'; // for strtoul to work
  char *endptr = nullptr;
  size_t size = (size_t)strtoul(s + *pos, &endptr, 10);
  if (*endptr != '\0' || size == 0)
    throw Parser::ParserError("invalid length");

  *cr = '\r';
  *pos = cr - s + 2;
  return size;
}