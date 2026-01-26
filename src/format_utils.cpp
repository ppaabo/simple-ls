#include <iomanip>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <time.h>

std::string format_time(time_t mtime) {
  std::tm *local_tm = localtime(&mtime);
  std::ostringstream oss;
  oss << std::put_time(local_tm, "%b %d %H:%M");
  return oss.str();
}
