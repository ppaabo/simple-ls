#pragma once

#include <string>

class Directory {
public:
  Directory(const std::string &path = ".");
  void list_entries();

private:
  std::string path_;
};
