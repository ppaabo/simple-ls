#pragma once

#include <string>

// settings based on flags
struct Settings {
  bool show_hidden = false; // -a, --all
};

void ls(const std::string &path, const Settings &settings);
