#include "format_utils.hpp"
#include "file.hpp"
#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <time.h>
#include <vector>

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
