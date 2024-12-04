// Copyright (c) 2024 Syntax Errors.
#include <vector>

#include "./util.hpp"

int main() {
  var string = "Hello world";
  var integer = Integer(123);
  var duble = 12.7;
  var boolean = false;


  std::vector<var> variables = std::vector<var>({string, integer, duble, boolean});

  for (auto variable : variables) {
    std::cout << variable << std::endl;
  }

  integer = "I do not want to do this";
  std::cout << integer << std::endl;

  var one = 1;
  var twoPointTree = 2.3;
  var result = one - twoPointTree;
  var second_one = 1;
  std::cout << "Result: " << result << std::endl;
  std::cout << one << " == " << second_one << "?: " << (one == second_one) << std::endl;

  if (one < twoPointTree) {
    std::cout << "True" << std::endl;
  }
  var newString = "New string";

  std::cout << "'" << newString << "' is ";
  if (newString < integer) {
    std::cout << "less";
  } else if (newString > integer) {
    std::cout << "greater";
  } else if (newString <= integer) {
    std::cout << "less or equal";
  } else if (newString >= integer) {
    std::cout << "greater or equal";
  } else if (newString == integer) {
    std::cout << "Equal";
  }
  std::cout << " than '" << integer << "'" << std::endl;

  if (newString == "New string") {
    std::cout << "Can compare strings to const char*" << std::endl;
  }
  return 0;
  // std::cout << integer + duble << std::endl;
}

// templates, list, tuple, dict, add
