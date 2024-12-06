// Copyright (c) 2024 Syntax Errors.
#include <vector>
#include <iostream>

#include "./util.hpp"

int main() {
  // Variables of different types
  var string = "Hello world";
  var integer = 123;
  var floating = 123.456;
  var boolean = false;

  // List
  var list = List();

  list->Call("append", {string.getValue()});
  list->Call("append", {integer.getValue()});
  list->Call("append", {floating.getValue()});
  list->Call("append", {boolean.getValue()});

  // All of this is broken for some reason
  // list->print(std::cout);
  // for (const auto& item : list) {
  //   std::cout << "Item: " << item << std::endl;
  // }

  if (list->Call("has", {})) {
    std::cout << "Found!" << std::endl;
  } else {
    std::cout << "Not found" << std::endl;
  }
  return 0;
}

// templates, list, tuple, dict, add
