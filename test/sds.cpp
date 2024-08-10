#include "sds.h"
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstring>
#include <gtest/gtest.h>
#include <string>

class SdsTest : public testing::Test {
protected:
  sds s;
  const size_t initSize;

  SdsTest() : initSize(20) { s = sdsAlloc(initSize); }
};

TEST_F(SdsTest, Allocation) {
  ASSERT_NE(s, nullptr);

  sdshdr *hdr = sdsGetHeader(s);
  EXPECT_EQ(hdr->len, 0);
  EXPECT_EQ(hdr->free, initSize);
}

TEST_F(SdsTest, GetLength) { EXPECT_EQ(sdsLen(s), 0); }

TEST_F(SdsTest, GetAvailable) { EXPECT_EQ(sdsAvail(s), initSize); }

TEST_F(SdsTest, SetLengthLessThanSize) {
  size_t newl = initSize / 2;
  sdsSetLen(s, newl);

  EXPECT_EQ(sdsLen(s), newl);
}

TEST_F(SdsTest, SetLengthMoreThanSize) {
  size_t newl = initSize + 20;

  EXPECT_ANY_THROW(sdsSetLen(s, newl));
}

TEST_F(SdsTest, CatFullSize) {
  std::vector<char> cmp;
  char c = 1;

  for (size_t _ = 0; _ < initSize; _++) {
    cmp.push_back(c);
    EXPECT_EQ(sdsCat(s, &c, sizeof(c)), sizeof(c));
    if (c + 1 < CHAR_MAX)
      c++;
  }

  EXPECT_EQ(sdsLen(s), initSize);
  EXPECT_EQ(sdsAvail(s), 0);

  for (size_t i = 0; i < initSize; i++)
    ASSERT_EQ(cmp[i], s[i]);
}

TEST_F(SdsTest, ShiftL) {
  std::string str = "123456*789";
  assert(str.length() <= initSize);

  sdsCat(s, str.c_str(), str.length());

  sdsShiftL(s, 6);

  EXPECT_EQ(s[0], '*');
  EXPECT_EQ(sdsLen(s), 4);
}
