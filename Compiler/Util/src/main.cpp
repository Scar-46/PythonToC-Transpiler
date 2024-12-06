// Copyright (c) 2024 Syntax Errors.
#include <vector>
#include <iostream>

#include "./util.hpp"

void checkReference(unused const Object& op1, unused const Object& op2) {
  std::cout << "Can be passed as object references" << std::endl;
}

void checkPointer(unused const ObjectPtr& op1, unused const ObjectPtr& op2) {
  std::cout << "Can be passed as object pointers" << std::endl;
}

int main() {
  var string = "Hello world";
  var integer = 123;
  var floating = 12.7;
  var boolean = false;
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
