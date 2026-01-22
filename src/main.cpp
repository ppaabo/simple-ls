#include "directory.hpp"
#include "file.hpp"
#include <exception>
#include <iostream>

void print_file_info(const File &file) {
  std::cout << file.get_type() << file.get_permissions() << " "
            << file.get_name() << " " << file.get_size() << "\n";
}

int main(int argc, char *argv[]) {
  try {
    Directory dir(".");
    for (const auto &file : dir.get_entries()) {
      print_file_info(file);
    }
  } catch (const std::exception &e) {
    std::cerr << "error occured:  " << e.what() << "\n";
  }
}
