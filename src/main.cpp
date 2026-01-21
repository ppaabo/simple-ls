#include "directory.hpp"
#include <exception>
#include <iostream>

int main(int argc, char *argv[]) {
  try {
    Directory dir(".");
    for (const auto &file : dir.get_entries()) {
      std::cout << file.get_name() << "\n";
    }
  } catch (const std::exception &e) {
    std::cerr << "error occured:  " << e.what() << "\n";
  }
}
