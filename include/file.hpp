#pragma once

#include <string>

class File {
public:
  File(const std::string &name, const std::string &path);
  const std::string &get_name() const;

private:
  std::string name_;
  std::string full_path_;
};
