#ifndef SDS_H
#define SDS_H

/* similar to redis strings implementation
 * see:
 * https://redis.io/docs/latest/operate/oss_and_stack/reference/internals/internals-sds/
 */

#include <cstddef>
typedef char *sds;

struct __attribute__((packed)) sdshdr {
  size_t len;
  size_t free;
  char buf[];
};

sdshdr *sdsGetHeader(sds s);
size_t sdsLen(sds s);
size_t sdsAvail(sds s);
void sdsSetLen(sds s, size_t newLen);
sds sdsAlloc(size_t size);
void sdsFree(sds s);
/**
 * returns the number of bytes written
 */
size_t sdsCat(sds dst, const char *src, size_t size);
/**
 * shifts the string n times to the left.
 * sets and returns the new length.
 */
size_t sdsShiftL(sds s, size_t n);

#endif
