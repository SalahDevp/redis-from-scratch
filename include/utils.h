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

char *findChar(char *buffer, size_t buffer_size, char target);
} // namespace utils

#endif
