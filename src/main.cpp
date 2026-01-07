#include "directory.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  Directory dir(".");
  dir.list_entries();
}
