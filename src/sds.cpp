#include "sds.h"
#include <cstdlib>

size_t getSdsLen(sds s) {
  return (size_t) * (s - sizeof(sdshdr::free) - sizeof(sdshdr::len));
}

size_t getSdsAvail(sds s) { return (size_t) * (s - sizeof(sdshdr::free)); }

sds allocSds(size_t size) {
  struct sdshdr *hdr =
      (struct sdshdr *)new char[sizeof(struct sdshdr) + size + 1];

  hdr->free = size;
  hdr->len = 0;

  return (sds)hdr->buf;
}

void freeSds(sds s) {
  struct sdshdr *hdr =
      (struct sdshdr *)s - sizeof(sdshdr::free) - sizeof(sdshdr::len);

  delete[] hdr;
}
