#ifndef UTILS_H
#define UTILS_H

namespace utils {

void die(const char *msg);

/**
 * sets fd to non blocking mode
 */
void fd_set_nb(int fd);

} // namespace utils

#endif
