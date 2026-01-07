#include "directory.hpp"
#include <cstring>
#include <dirent.h>
#include <iostream>
#include <stdexcept>
#include <string>

Directory::Directory(const std::string &path) : path_(path) {}

void Directory::list_entries() {
  DIR *dir = opendir(path_.c_str());

  if (!dir) {
    throw std::runtime_error(std::string("Error opening directory: ") +
                             std::strerror(errno));
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != nullptr) {
    // skip . and ..
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
      std::cout << entry->d_name << "\n";
    }
  }
  closedir(dir);
}
