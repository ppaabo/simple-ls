#include "argparse.hpp"
#include "directory.hpp"
#include "file.hpp"
#include <exception>
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

void print_file_info(const File &file) {
  std::cout << file.get_type() << file.get_permissions() << " "
            << file.get_owner_username() << " " << file.get_owner_groupname()
            << " " << file.get_name() << " " << file.get_size() << "\n";
}

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("simple-ls");
  program.add_argument("path").help("Directory path").default_value(".");

  try {
    program.parse_args(argc, argv);
  } catch (const std::exception &e) {
    std::cerr << "error occured during argument parsing: " << e.what() << "\n";
    std::cerr << program;
    return 1;
  }

  try {
    std::string path = program.get<std::string>("path");
    if (!fs::exists(path) || !fs::is_directory(path)) {
      throw std::runtime_error("provided path is invalid: " + path);
    }
    Directory dir(path);
    for (const auto &file : dir.get_entries()) {
      print_file_info(file);
    }
  } catch (const std::exception &e) {
    std::cerr << "error occured: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
