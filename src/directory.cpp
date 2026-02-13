#include "directory.hpp"
#include "error_collector.hpp"
#include <cstring>
#include <dirent.h>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <vector>

Directory::Directory(const std::string &path, ErrorCollector &errors)
    : path_(path) {
  read_entries(errors);
}

const std::vector<File> &Directory::get_entries() const { return entries_; }

void Directory::read_entries(ErrorCollector &errors) {
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
      // store message if constructing a file instance fails
      errors.add_error(std::string("failed to read ") + entry->d_name + ": " +
                       e.what());
    }
  }
  closedir(dir);
}
