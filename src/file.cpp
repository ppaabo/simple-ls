#include "file.hpp"
#include <cstring>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

File::File(const std::string &name, const std::string &dir)
    : name_(name), full_path_(dir + "/" + name), stat_{} {
  retrieve_attributes();
}

const std::string &File::get_name() const { return name_; }
off_t File::get_size() const { return stat_.st_size; }
std::string File::get_permissions() const {
  return get_owner_permissions() + get_group_permissions() +
         get_other_permissions();
}
char File::get_type() const {
  if (S_ISREG(stat_.st_mode))
    return '-';
  if (S_ISDIR(stat_.st_mode))
    return 'd';
  if (S_ISLNK(stat_.st_mode))
    return 'l';
  if (S_ISBLK(stat_.st_mode))
    return 'b';
  if (S_ISCHR(stat_.st_mode))
    return 'c';
  if (S_ISFIFO(stat_.st_mode))
    return 'p';
  if (S_ISSOCK(stat_.st_mode))
    return 's';

  return '?';
}

void File::retrieve_attributes() {
  struct stat buf;
  int result = stat(full_path_.c_str(), &buf);

  if (result == -1) {
    throw std::runtime_error(std::string("stat failed for ") + full_path_ +
                             ": " + strerror(errno));
  }
  stat_ = buf;
}

std::string File::get_owner_permissions() const {
  std::string result;
  result += (stat_.st_mode & S_IRUSR) ? "r" : "-";
  result += (stat_.st_mode & S_IWUSR) ? "w" : "-";
  result += (stat_.st_mode & S_IXUSR) ? "x" : "-";
  return result;
}

std::string File::get_group_permissions() const {
  std::string result;
  result += (stat_.st_mode & S_IRGRP) ? "r" : "-";
  result += (stat_.st_mode & S_IWGRP) ? "w" : "-";
  result += (stat_.st_mode & S_IXGRP) ? "x" : "-";
  return result;
}

std::string File::get_other_permissions() const {
  std::string result;
  result += (stat_.st_mode & S_IROTH) ? "r" : "-";
  result += (stat_.st_mode & S_IWOTH) ? "w" : "-";
  result += (stat_.st_mode & S_IXOTH) ? "x" : "-";
  return result;
}
