#include "argparse.hpp"
#include "file_utils.hpp"
#include <exception>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("simple-ls");
  program.add_argument("path").help("Directory path").default_value(".");
  program.add_argument("-a", "--all")
      .help("Show files starting with '.'")
      .flag();
  program.add_argument("-A", "--almost-all")
      .help("Do not show implied '.' and '..'")
      .flag();
  program.add_argument("-l").help("Print entries using long format").flag();
  program.add_argument("-p").help("Add '/' indicator to directories").flag();
  program.add_argument("-g", "--group-directories-first")
      .help("Group directories before files")
      .flag();

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &e) {
    std::cerr << "error occured during argument parsing: " << e.what() << "\n";
    std::cerr << program;
    return 1;
  }

  try {
    Settings settings{};
    std::string path = program.get<std::string>("path");
    if (!fs::exists(path) || !fs::is_directory(path)) {
      throw std::runtime_error("provided path is invalid: " + path);
    }
    settings.show_hidden = program.get<bool>("-a");
    settings.show_almost_all = program.get<bool>("-A");
    settings.print_long_format = program.get<bool>("-l");
    settings.print_dir_indicator = program.get<bool>("-p");
    settings.group_directories_first = program.get<bool>("-g");
    ls(path, settings);
  } catch (const std::exception &e) {
    std::cerr << "error occured: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
