// Copyright (c) 2024 Syntax Errors.
#include <vector>

#include "./util.hpp"

int main() {
  var string = "Hello world";
  var integer = 123;

  std::vector<var> variables = std::vector<var>({string, integer});

  for (auto variable : variables) {
    std::cout << variable << std::endl;
  }

  integer = "I do not want to do this";
  std::cout << integer << std::endl;

  for (auto variable : variables) {
    std::cout << variable << std::endl;
  }
}

# templates, list, tuple, dict, add