#include "DataStore.h"

void DataStore::set(const std::string &key, const std::string &value) {
  smap[key] = value;
}

const std::string &DataStore::get(const std::string &key) {
  auto it = smap.find(key);
  if (it == smap.end())
    throw KeyNotFoundError(key);

  return it->second;
}
