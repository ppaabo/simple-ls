#pragma once

#include <ostream>
#include <string>
#include <vector>

class ErrorCollector {
public:
  void add_error(const std::string &message);
  bool has_errors() const;
  const std::vector<std::string> &get_errors() const;
  void print_errors(std::ostream &os) const;

private:
  std::vector<std::string> errors_;
};
