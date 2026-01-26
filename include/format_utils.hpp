#include <string>
#include <sys/types.h>
#include <time.h>

// convert time_t to formatted local time string
std::string format_time(time_t mtime);
