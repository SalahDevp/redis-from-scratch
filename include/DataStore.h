#ifndef DATA_STORE_H
#define DATA_STORE_H

#include <stdexcept>
#include <string>
#include <unordered_map>

class DataStore {
public:
  void set(const std::string &key, const std::string &value);
  const std::string &get(const std::string &key);
  /**
   * returns true if key is deleted
   */
  bool del(const std::string &key);

  class KeyNotFoundError : public std::runtime_error {
  public:
    KeyNotFoundError(const std::string &key)
        : std::runtime_error("key: " + key + " doesn't exist") {}
  };

private:
  std::unordered_map<std::string, std::string> smap;
};

#endif
