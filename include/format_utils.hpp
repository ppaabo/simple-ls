#include "file.hpp"
#include <cstddef>
#include <string>
#include <sys/types.h>
#include <time.h>
#include <vector>

constexpr const char *COLOR_RESET_CODE = "\033[0m";

struct ColWidths {
  size_t owner = 0;
  size_t group = 0;
  size_t file_size = 0;
};

// convert time_t to formatted local time string
std::string format_time(time_t mtime);

// calculate column widths for formatting
ColWidths calc_col_widths(const std::vector<File> &entries);

std::string get_color_code(char file_type, bool is_executable = false);
