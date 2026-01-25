#pragma once

#include <string>

// settings based on flags
struct Settings {
  bool show_hidden = false;       // -a, --all
  bool print_long_format = false; // -l
};

void ls(const std::string &path, const Settings &settings);
