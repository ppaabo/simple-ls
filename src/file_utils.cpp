#include "file_utils.hpp"
#include "directory.hpp"
#include "file.hpp"
#include "format_utils.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

namespace {
void filter_hidden(std::vector<File> &entries, const Settings &settings) {
  if (settings.show_hidden)
    return;

  // -A, remove '.' and '..'
  if (settings.show_almost_all) {
    auto new_end =
        std::remove_if(entries.begin(), entries.end(), [](const File &f) {
          const std::string &name = f.get_name();
          return name == "." || name == "..";
        });
    entries.erase(new_end, entries.end());
    return;
  }

  // remove entries starting with '.'
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

void print_file_info(const std::vector<File> &entries, const Settings &settings,
                     std::ostream &os) {
  for (const File &file : entries) {
    std::string file_name = file.get_name();
    const char file_type = file.get_type();

    if (settings.print_dir_indicator && file.is_directory()) {
      file_name += "/";
    }

    if (!settings.print_long_format) {
      os << get_color_code(file_type) << file_name << COLOR_RESET_CODE << "\n";
      continue;
    }

    ColWidths widths = calc_col_widths(entries);
    os << file_type << file.get_permissions() << " " << std::setw(widths.owner)
       << file.get_owner_username() << " " << std::setw(widths.group)
       << file.get_owner_groupname() << " " << std::setw(widths.file_size)
       << file.get_size() << " " << format_time(file.get_last_mtime()) << " "
       << get_color_code(file_type) << file_name << COLOR_RESET_CODE << "\n";
  }
}
} // namespace

void ls(const std::string &path, const Settings &settings) {
  try {
    Directory dir(path);
    std::vector<File> entries = dir.get_entries();
    filter_hidden(entries, settings);
    sort_entries(entries, settings);
    print_file_info(entries, settings, std::cout);
  } catch (const std::exception &e) {
    throw;
  }
}
