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
  var map = Map();

  map.as<Map>()->addPair({string, floating});
  map.as<Map>()->addPair({integer, boolean});

  list->Call("append", {string.getValue()});
  list->Call("append", {integer.getValue()});
  list->Call("append", {floating.getValue()});
  list->Call("append", {boolean.getValue()});

  std::cout << "List: " << list << std::endl;
  // All of this is broken for some reason
  for (const auto& item : list) {
    std::cout << "Item: " << item << std::endl;
  }

  if (auto val = var(list->Call("has", {string.getValue()}))) {
    std::cout << "Found! " << val << std::endl;
  } else {
    std::cout << "Not found" << std::endl;
  }

  std::cout << "Map: " << map << std::endl << " | Keys: " << var(map->Call("keys", {}))
    << std::endl << " | Values: " << var(map->Call("keys", {})) << std::endl;

  // var slice = list->Call("slice", {Integer(0), Integer(1)})->Call("has", {string.getValue()});
  var slice = list->Call("slice", {Integer(0), Integer(1)});
  var query = slice->Call("has", {string.getValue()});
  std::cout << "List slice: " << slice[Integer(0)] << std::endl;
  std::cout << "List query: " << query << std::endl;

  slice = string->Call("slice", {Integer(0), Integer(5)});
  std::cout << "String slice: " << slice << std::endl;

  // var slice = list->Call("slice", {Integer(4)});
  // std::cout << "Slice: " << slice << std::endl;
  // std::cout << "String: " << var(string->Call("slice", {Integer(0), Integer(5)}));
  return 0;
}

// templates, list, tuple, dict, add
