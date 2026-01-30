#include "format_utils.hpp"
#include "file.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <time.h>
#include <unordered_map>
#include <vector>

namespace {
const std::unordered_map<std::string, std::string> default_color_map = {
    {"fi", ""},               // regular file (no color)
    {"di", "\033[01;34m"},    // directory
    {"ln", "\033[01;36m"},    // symlink
    {"bd", "\033[40;33;01m"}, // block device
    {"cd", "\033[40;33;01m"}, // char device
    {"pi", "\033[40;33m"},    // FIFO
    {"so", "\033[01;35m"},    // socket
};

std::unordered_map<std::string, std::string>
parse_ls_colors(const std::string &lsColors) {
  std::unordered_map<std::string, std::string> colorMap;
  std::istringstream ss(lsColors);
  std::string pair;
  while (std::getline(ss, pair, ':')) {
    size_t pos = pair.find('=');
    if (pos != std::string::npos) {
      std::string key = pair.substr(0, pos);
      std::string value = pair.substr(pos + 1);
      // store colors only for specified types
      if (default_color_map.find(key) != default_color_map.end()) {
        colorMap[key] = "\033[" + value + "m";
      }
    }
  }
  // fill colors for missing types from default
  for (const auto &[key, code] : default_color_map) {
    if (colorMap.find(key) == colorMap.end())
      colorMap[key] = code;
  }
  return colorMap;
}

const std::unordered_map<std::string, std::string> &get_color_map() {
  static std::unordered_map<std::string, std::string> colorMap = [] {
    const char *env = secure_getenv("LS_COLORS");
    if (!env)
      return default_color_map;
    return parse_ls_colors(env);
  }();
  return colorMap;
}
} // namespace

std::string format_time(time_t mtime) {
  std::tm *local_tm = localtime(&mtime);
  std::ostringstream oss;
  oss << std::put_time(local_tm, "%b %d %H:%M");
  return oss.str();
}

ColWidths calc_col_widths(const std::vector<File> &entries) {
  ColWidths widths{};
  // MAX to not break formatting cause of single outlier valeu
  const size_t MAX_COL_WIDTH = 10;

  for (const auto &file : entries) {
    widths.owner =
        std::min(MAX_COL_WIDTH,
                 std::max(widths.owner, file.get_owner_username().size()));
    widths.group =
        std::min(MAX_COL_WIDTH,
                 std::max(widths.group, file.get_owner_groupname().size()));
    widths.file_size = std::min(
        MAX_COL_WIDTH,
        std::max(widths.file_size, std::to_string(file.get_size()).size()));
  }
  return widths;
}

std::string get_color_code(char file_type) {
  const auto &colors = get_color_map();
  switch (file_type) {
  case '-':
    return colors.at("fi");
  case 'd':
    return colors.at("di");
  case 'l':
    return colors.at("ln");
  case 'b':
    return colors.at("bd");
  case 'c':
    return colors.at("cd");
  case 'p':
    return colors.at("pi");
  case 's':
    return colors.at("so");
  case 'r':
    return COLOR_RESET_CODE; // explicit reset
  default:
    return COLOR_RESET_CODE; // fallback
  }
}
