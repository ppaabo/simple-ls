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
time_t File::get_last_mtime() const { return stat_.st_mtime; }
bool File::is_directory() const { return S_ISDIR(stat_.st_mode); }

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

std::string File::get_permissions() const {
  return get_owner_permissions() + get_group_permissions() +
         get_other_permissions();
}

std::string File::get_owner_username() const { return owner_username_; }

std::string File::get_owner_groupname() const { return owner_groupname_; }

void File::retrieve_attributes() {
  struct stat stat_buf;
  int stat_result = stat(full_path_.c_str(), &stat_buf);

  if (stat_result == -1) {
    throw std::runtime_error(std::string("stat failed for ") + full_path_ +
                             ": " + strerror(errno));
  }
  stat_ = stat_buf;

  // get owner usename
  struct passwd pw_result;
  struct passwd *pw_resultp = nullptr;
  long pw_bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
  if (pw_bufsize == -1)
    pw_bufsize = 16384;
  std::vector<char> pw_buf(static_cast<size_t>(pw_bufsize));
  int pw_ret = getpwuid_r(stat_.st_uid, &pw_result, pw_buf.data(),
                          pw_buf.size(), &pw_resultp);
  if (pw_ret != 0 || pw_resultp == nullptr) {
    owner_username_ = "-";
  } else {
    owner_username_ = pw_result.pw_name;
  }

  // get groupname
  struct group gr_result;
  struct group *gr_resultp = nullptr;
  long gr_bufsize = sysconf(_SC_GETGR_R_SIZE_MAX);
  if (gr_bufsize == -1)
    gr_bufsize = 16384;
  std::vector<char> gr_buf(static_cast<size_t>(gr_bufsize));
  int ret = getgrgid_r(stat_.st_gid, &gr_result, gr_buf.data(), gr_buf.size(),
                       &gr_resultp);
  if (ret != 0 || gr_resultp == nullptr) {
    owner_groupname_ = "-";
  } else {
    owner_groupname_ = gr_result.gr_name;
  }
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
