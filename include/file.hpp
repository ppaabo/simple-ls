#pragma once

#include <string>
#include <sys/stat.h>
#include <sys/types.h>

class File {
public:
  File(const std::string &name, const std::string &path);
  const std::string &get_name() const;
  off_t get_size() const;

private:
  std::string name_;
  std::string full_path_;
  struct stat stat_;

  void retrieve_attributes();
};
