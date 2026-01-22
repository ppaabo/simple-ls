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

void File::retrieve_attributes() {
  struct stat buf;
  int result = stat(full_path_.c_str(), &buf);

  if (result == -1) {
    throw std::runtime_error(std::string("stat failed for ") + full_path_ +
                             ": " + strerror(errno));
  }
  stat_ = buf;
}
