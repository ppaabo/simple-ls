#pragma once

#include "error_collector.hpp"
#include <string>

// settings based on flags
struct Settings {
  bool show_hidden = false;             // -a, --all
  bool show_almost_all = false;         // -A, --almost-all
  bool print_long_format = false;       // -l
  bool print_dir_indicator = false;     // -p
  bool group_directories_first = false; // -g, --group_directories_first
  bool verbose = false;
};

void ls(const std::string &path, const Settings &settings,
        ErrorCollector &errors);
