#pragma once

#include "file.hpp"
#include <string>
#include <vector>

class Directory {
public:
  Directory(const std::string &path = ".");
  const std::vector<File> &get_entries() const;
  void list_entries();

private:
  std::string path_;
  std::vector<File> entries_;
  void read_entries();
};
