#pragma once

#include <string>

// settings based on flags
struct Settings {
  bool show_hidden = false;         // -a, --all
  bool print_long_format = false;   // -l
  bool print_dir_indicator = false; // -p
};

void ls(const std::string &path, const Settings &settings);
