#include "file_utils.hpp"
#include "directory.hpp"
#include "file.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace {
void print_file_info(const File &file) {
  std::cout << file.get_type() << file.get_permissions() << " "
            << file.get_owner_username() << " " << file.get_owner_groupname()
            << " " << file.get_name() << " " << file.get_size() << "\n";
}

void filter_hidden(std::vector<File> &entries) {
  auto new_end =
      std::remove_if(entries.begin(), entries.end(), [](const File &f) {
        const std::string &name = f.get_name();
        return name[0] == '.';
      });
  entries.erase(new_end, entries.end());
}
} // namespace

void ls(const std::string &path, const Settings &settings) {
  try {
    Directory dir(path);
    std::vector<File> entries = dir.get_entries();
    if (!settings.show_hidden) {
      filter_hidden(entries);
    }
    for (const auto &file : entries) {
      print_file_info(file);
    }
  } catch (const std::exception &e) {
    throw;
  }
}
