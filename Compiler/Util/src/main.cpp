// Copyright (c) 2024 Syntax Errors.
#include <vector>
#include <iostream>

#include "./util.hpp"

int main() {
  // Variables of different types
  var hello_world = "Hello world";
  var number = 123;
  
  // List
  var list = List();

  list->Call("append", {string.getValue()});
  list->Call("append", {integer.getValue()});
  list->Call("append", {floating.getValue()});
  list->Call("append", {boolean.getValue()});

  var result = list->Call("slice", {Integer(-1)});

  std::cout << "Result: " << result;
  return 0;
}

// templates, list, tuple, dict, add
