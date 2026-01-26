#include "file_utils.hpp"
#include "directory.hpp"
#include "file.hpp"
#include "format_utils.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

namespace {
void filter_hidden(std::vector<File> &entries) {
  auto new_end =
      std::remove_if(entries.begin(), entries.end(), [](const File &f) {
        const std::string &name = f.get_name();
        return name[0] == '.';
      });
  entries.erase(new_end, entries.end());
}

void sort_entries(std::vector<File> &entries, const Settings &settings) {
  std::sort(entries.begin(), entries.end(), [](const File &a, const File &b) {
    return a.get_name() < b.get_name();
  });
  // keep alphabetical sorting and group directories before files
  if (settings.group_directories_first) {
    std::stable_partition(entries.begin(), entries.end(),
                          [](const File &f) { return f.is_directory(); });
  }
}

void print_file_info(const File &file, const Settings &settings,
                     std::ostream &os) {
  std::string display_name = file.get_name();
  if (settings.print_dir_indicator && file.is_directory()) {
    display_name += "/";
  }

  if (!settings.print_long_format) {
    os << display_name << "\n";
    return;
  }

  os << file.get_type() << file.get_permissions() << " "
     << file.get_owner_username() << " " << file.get_owner_groupname() << " "
     << file.get_size() << " " << format_time(file.get_last_mtime()) << " "
     << display_name << "\n";
}
} // namespace

void ls(const std::string &path, const Settings &settings) {
  try {
    Directory dir(path);
    std::vector<File> entries = dir.get_entries();
    if (!settings.show_hidden) {
      filter_hidden(entries);
    }
    sort_entries(entries, settings);
    for (const auto &file : entries) {
      print_file_info(file, settings, std::cout);
    }
  } catch (const std::exception &e) {
    throw;
  }
}
