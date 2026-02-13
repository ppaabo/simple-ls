#include "error_collector.hpp"
#include <ostream>
#include <string>
#include <vector>

void ErrorCollector::add_error(const std::string &message) {
  errors_.push_back(message);
}

bool ErrorCollector::has_errors() const { return !errors_.empty(); }

const std::vector<std::string> &ErrorCollector::get_errors() const {
  return errors_;
}

void ErrorCollector::print_errors(std::ostream &os) const {
  for (const auto &error : errors_) {
    os << error << "\n";
  }
}
