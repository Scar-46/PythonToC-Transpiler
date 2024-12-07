// Copyright (c) 2024 Syntax Errors.
#include <vector>
#include <iostream>

#include "./util.hpp"

void TestVar() {
  printf("\n\n========== VAR ==========\n\n");
  var string = "Hello world";
  var integer = 123;
  var floating = 123.456;
  var boolean = false;

  printf("Done\n");
}

void TestList() {
  printf("\n\n========== LIST ==========\n\n");

  var list = List();

  list->Call("append", {String("Bring")});
  list->Call("append", {Integer(2)});
  list->Call("append", {String("friends")});
  list->Call("insert", {Integer(0), String("No!")});
  list->Call("insert", {list->Call("__len__", {}), String("to the party")});
  list->Call("append", {Boolean(true)});

  std::cout << "List: " << list << std::endl;
  for (const auto& item : list) {
    std::cout << "Item: " << item << std::endl;
  }

  var slice = list->Call("slice", {Integer(0), Integer(1)});
  std::cout << "List slice: " << slice[Integer(0)] << std::endl;
  
  var query = String("friends");
  if (auto val = var(slice->Call("index", {query.getValue()}))) {
    std::cout << query << " found in slice! At index " << val << std::endl;
  } else {
    std::cout << query  << " not found in slice" << std::endl;
  }
}

void TestString() {
  printf("\n\n========== STRING ==========\n\n");

  var string = "Hello World";
  std::cout << "String: " << string << std::endl;

  var slice = string->Call("slice", {Integer(0), Integer(5)});
  std::cout << "String slice: " << slice << std::endl;
}

void TestSet() {
  printf("\n\n========== SET ==========\n\n");

  var set = Set();

  set->Call("add", {Integer(0)});
  set->Call("add", {Integer(100)});
  set->Call("add", {String("Zero")});
  set->Call("add", {String("Hundred")});

  std::cout << "Set: " << set << std::endl;
  for (const auto& item : set) {
    std::cout << "Set item: " << item << std::endl;
  }
}

void TestMap() {
  printf("\n\n========== MAP ==========\n\n");

  var map = Map();

  map->Call("addElement", {String("Hello"), String("World")});
  map->Call("addElement", {Integer(1), Double(3.1415)});

  std::cout << "Map: " << map << std::endl << " | Keys: " << var(map->Call("keys", {}))
    << std::endl << " | Values: " << var(map->Call("values", {})) << std::endl;

  var query = String("Something impossible");
  if (auto val = var(map->Call("get", {query.getValue()}))) {
    std::cout << query << " has assigned a value in map: " << val << std::endl;
  } else {
    std::cout << query  << " not found in map" << std::endl;
  }
}

int main() {
  // Variables of different types
  TestVar();
  TestString();
  TestList();
  TestSet();
  TestMap();

  return 0;
}