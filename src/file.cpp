#include "file.hpp"
#include <cstring>
#include <grp.h>
#include <pwd.h>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

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

std::string File::get_owner_username() const {
  struct passwd result;
  struct passwd *resultp = nullptr;
  long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX); // recommended buf size
  if (bufsize == -1) {
    bufsize = 16384;
  }

  std::vector<char> buf(static_cast<size_t>(bufsize));
  int ret = getpwuid_r(stat_.st_uid, &result, buf.data(), buf.size(), &resultp);
  if (ret != 0) {
    throw std::runtime_error(std::string("getpwuid_r failed: ") +
                             std::strerror(ret));
  }
  if (resultp == nullptr) {
    return "-"; // placeholder if username not found
  }

  return std::string(result.pw_name);
}

std::string File::get_owner_groupname() const {
  struct group result;
  struct group *resultp = nullptr;
  long bufsize = sysconf(_SC_GETGR_R_SIZE_MAX); // recommended buf size
  if (bufsize == -1) {
    bufsize = 16384;
  }

  std::vector<char> buf(static_cast<size_t>(bufsize));
  int ret = getgrgid_r(stat_.st_gid, &result, buf.data(), buf.size(), &resultp);
  if (ret != 0) {
    throw std::runtime_error(std::string("getgrgid_r failed: ") +
                             std::strerror(ret));
  }
  if (resultp == nullptr) {
    return "-"; // placeholder if groupname not found
  }

  return std::string(result.gr_name);
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
