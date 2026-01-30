#include "directory.hpp"
#include <cstring>
#include <dirent.h>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <vector>

Directory::Directory(const std::string &path) : path_(path) { read_entries(); }

const std::vector<File> &Directory::get_entries() const { return entries_; }

void Directory::read_entries() {
  DIR *dir = opendir(path_.c_str());
  if (!dir) {
    throw std::runtime_error(std::string("Error opening directory: ") +
                             std::strerror(errno));
  }

  struct dirent *entry;
  errno = 0;
  while ((entry = readdir(dir)) != nullptr) {
    entries_.emplace_back(entry->d_name, path_);
  }
  if (errno != 0) {
    closedir(dir);
    throw std::runtime_error(std::string("Errro reading directory: ") +
                             strerror(errno));
  }
  closedir(dir);
}
