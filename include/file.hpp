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
  std::string owner_username_;
  std::string owner_groupname_;

  void init_stat();
  void init_owner_username();
  void init_owner_groupname();

  std::string get_owner_permissions() const;
  std::string get_group_permissions() const;
  std::string get_other_permissions() const;
};
