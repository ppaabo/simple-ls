#pragma once

#include "error_collector.hpp"
#include "file.hpp"
#include <string>
#include <vector>

class Directory {
public:
  Directory(const std::string &path, ErrorCollector &errors);
  const std::vector<File> &get_entries() const;
  void list_entries();

private:
  std::string path_;
  std::vector<File> entries_;
  void read_entries(ErrorCollector &errors);
};
