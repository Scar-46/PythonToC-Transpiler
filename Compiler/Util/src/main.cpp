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

  map->Call("addElement", {string.getValue(), floating.getValue()});
  map->Call("addElement", {integer.getValue(), boolean.getValue()});
  
  list->Call("append", {string.getValue()});
  list->Call("append", {integer.getValue()});
  list->Call("append", {floating.getValue()});
  list->Call("append", {boolean.getValue()});
  list->Call("insert", {Integer(0), String("Wow")});
  list->Call("insert", {list->Call("size", {}), String("Help")});

  std::cout << "List: " << list << std::endl;
  // All of this is broken for some reason
  for (const auto& item : list) {
    std::cout << "Item: " << item << std::endl;
  }

  // Set
  var set = Set();
  set->Call("add", {Integer(0)});
  set->Call("add", {Integer(100)});
  set->Call("add", {String("Zero")});
  set->Call("add", {String("Hundred")});

  for (const auto& item : set) {
    std::cout << "Set item: " << item << std::endl;
  }

  if (auto val = var(list->Call("has", {string.getValue()}))) {
    std::cout << "Found! " << val << std::endl;
  } else {
    std::cout << "Not found" << std::endl;
  }

  std::cout << "Map: " << map << std::endl << " | Keys: " << var(map->Call("keys", {}))
    << std::endl << " | Values: " << var(map->Call("values", {})) << std::endl;

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
