#include "sds.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

sdshdr *sdsGetHeader(sds s) {
  return (struct sdshdr *)(s - sizeof(sdshdr::free) - sizeof(sdshdr::len));
}

size_t sdsLen(sds s) {
  return (size_t) * (s - sizeof(sdshdr::free) - sizeof(sdshdr::len));
}

void sdsSetLen(sds s, size_t newLen) {
  struct sdshdr *hdr = sdsGetHeader(s);
  if (hdr->len + hdr->free < newLen)
    throw std::runtime_error("sds length must be less then the allocated size");

  hdr->free -= newLen - hdr->len;
  hdr->len = newLen;
}

size_t sdsAvail(sds s) { return *((size_t *)(s - sizeof(sdshdr::free))); }

sds sdsAlloc(size_t size) {
  struct sdshdr *hdr =
      (struct sdshdr *)new char[sizeof(struct sdshdr) + size + 1];

  hdr->free = size;
  hdr->len = 0;

  return (sds)hdr->buf;
}

void sdsFree(sds s) {
  struct sdshdr *hdr = sdsGetHeader(s);

  delete[] hdr;
}

size_t sdsShiftL(sds s, size_t n) {
  size_t newln;
  if (n >= sdsLen(s)) {
    *s = '\0';
    sdsSetLen(s, 0);
    newln = 0;
  }

  else {
    newln = sdsLen(s) - n;
    memcpy(s, s + n, newln);
    sdsSetLen(s, newln);
  }
  return newln;
}

size_t sdsCat(sds dst, const char *src, size_t size) {
  size = std::min(size, sdsAvail(dst));

  memcpy(dst + sdsLen(dst), src, size);
  sdsSetLen(dst, sdsLen(dst) + size);

  return size;
}
