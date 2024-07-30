#ifndef UTILS_H
#define UTILS_H

#include <string>
namespace utils {

void die(const char *msg);

/**
 * sets fd to non blocking mode
 */
void fd_set_nb(int fd);

void strToLowerCase(std::string &s);

} // namespace utils

#endif
