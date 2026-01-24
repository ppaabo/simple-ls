#pragma once

#include <string>
#include <sys/stat.h>
#include <sys/types.h>

class File {
public:
  File(const std::string &name, const std::string &path);
  const std::string &get_name() const;
  off_t get_size() const;
  char get_type() const;
  time_t get_last_mtime() const;
  bool is_directory() const;
  std::string get_permissions() const;
  std::string get_owner_username() const;
  std::string get_owner_groupname() const;

private:
  std::string name_;
  std::string full_path_;
  struct stat stat_;

  void retrieve_attributes();

  std::string get_owner_permissions() const;
  std::string get_group_permissions() const;
  std::string get_other_permissions() const;
};
