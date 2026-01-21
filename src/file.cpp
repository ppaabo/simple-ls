#include "file.hpp"
#include <string>

File::File(const std::string &name, const std::string &dir)
    : name_(name), full_path_(dir + "/" + name) {}

const std::string &File::get_name() const { return name_; }
