#include "Parser.h"
#include "Connection.h"
#include "sds.h"
#include <cstddef>
#include <gtest/gtest.h>
#include <memory>
#include <string>

class ParserTest : public testing::Test {
protected:
  std::shared_ptr<Connection> conn;
  Parser parser;

  ParserTest() { conn = std::make_shared<Connection>(1); }
};

TEST_F(ParserTest, ParseCompleteQuery) {
  std::string query = "*3\r\n$3\r\nGET\r\n$3\r\nkey\r\n$5\r\nvalue\r\n";

  sdsCat(conn->query_buf, query.c_str(), query.length());
  bool res = parser.parseQuery(conn);

  EXPECT_EQ(res, true);
  EXPECT_EQ(conn->argv.size(), 3);
  EXPECT_EQ(conn->argv[0], "GET");
  EXPECT_EQ(conn->argv[1], "key");
  EXPECT_EQ(conn->argv[2], "value");
  EXPECT_EQ(conn->qpos, query.length()); // qpos at the end of the string
  EXPECT_EQ(conn->bs_ln, 0);             // bs_ln is reset after parsing
}

TEST_F(ParserTest, ParseIncompleteQuery) {
  // Incomplete query: missing last bulk string "value"
  std::string query = "*3\r\n$3\r\nGET\r\n$3\r\nkey\r\n$5\r\nval";

  sdsCat(conn->query_buf, query.c_str(), query.length());
  bool res = parser.parseQuery(conn);

  EXPECT_EQ(res, false); // Should return false due to incomplete query
  EXPECT_EQ(conn->argv.size(), 2); // Only two arguments parsed
  EXPECT_EQ(conn->argv[0], "GET");
  EXPECT_EQ(conn->argv[1], "key");

  size_t argInd = query.find("val");
  EXPECT_EQ(conn->qpos,
            argInd); // qpos should be at the start of the inparsed arg
}

TEST_F(ParserTest, ParseInvalidQuery_MissingStar) {
  // Invalid query: missing '*' at the beginning
  std::string query = "3\r\n$3\r\nGET\r\n$3\r\nkey\r\n$5\r\nvalue\r\n";

  sdsCat(conn->query_buf, query.c_str(), query.length());

  EXPECT_THROW(parser.parseQuery(conn), Parser::ParserError);
}

TEST_F(ParserTest, ParseInvalidQuery_MissingDollar) {
  // Invalid query: missing '$' before the bulk string
  std::string query = "*3\r\n$3\r\nGET\r\n$3\r\nkey\r\n5\r\nvalue\r\n";

  sdsCat(conn->query_buf, query.c_str(), query.length());

  EXPECT_THROW(parser.parseQuery(conn), Parser::ParserError);
}
