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

  var result = list->Call("has", {Double(123.432356)});

  std::cout << "Result: " << result << std::endl;
  return 0;
}

// templates, list, tuple, dict, add
