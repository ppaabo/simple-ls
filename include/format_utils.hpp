#include "file.hpp"
#include <cstddef>
#include <string>
#include <sys/types.h>
#include <time.h>
#include <vector>

struct ColWidths {
  size_t owner = 0;
  size_t group = 0;
  size_t file_size = 0;
};

// convert time_t to formatted local time string
std::string format_time(time_t mtime);

// calculate column widths for formatting
ColWidths calc_col_widths(const std::vector<File> &entries);
