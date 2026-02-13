#include "directory.hpp"
#include <cstring>
#include <dirent.h>
#include <iostream>
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
  while (true) {
    errno = 0; // reset errno before each readdir
    entry = readdir(dir);
    if (!entry) {
      if (errno != 0) {
        throw std::runtime_error(
            std::string("Error reading directory entry: ") + strerror(errno));
      }
      break; // end of dir or error
    }

    try {
      entries_.emplace_back(entry->d_name, path_);
    } catch (const std::exception &e) {
      std::cerr << "error occured: " << e.what() << "\n";
    }
  }
  closedir(dir);
}
