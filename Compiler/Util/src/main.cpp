// Copyright (c) 2024 Syntax Errors.
#include "./util.hpp"

int main() {
  // Variables of different types
  var hello_world = "Hello world";
  var number = 123;
  
  // List
  var list = List();
  
  std::cout << "BEFORE appending either var" << std::endl;
  std::cout << "List does " << 
    ((var) list.Call("has", {hello_world.getValue()}) ? "have " : "not have ") << "hello_world, and does " <<
    ((var) list.Call("has", {number.getValue()}) ? "have " : "not have ") << "number" 
    << std::endl;

  list.Call("append", {hello_world.getValue()});

  std::cout << "AFTER appending hello_world" << std::endl;
  std::cout << "List does " << 
    ((var) list.Call("has", {hello_world.getValue()}) ? "have " : "not have ") << "hello_world, and does " <<
    ((var) list.Call("has", {number.getValue()}) ? "have " : "not have ") << "number" 
    << std::endl;

  list.Call("append", {number.getValue()});

  std::cout << "AFTER appending number" << std::endl;
  std::cout << "List does " << 
    ((var) list.Call("has", {hello_world.getValue()}) ? "have " : "not have ") << "hello_world, and does " <<
    ((var) list.Call("has", {number.getValue()}) ? "have " : "not have ") << "number" 
    << std::endl;

  hello_world = "Goodbye world";

  std::cout << "AFTER changing hello_world" << std::endl;
  std::cout << "List does " << 
    ((var) list.Call("has", {hello_world.getValue()}) ? "have " : "not have ") << "hello_world, and does " <<
    ((var) list.Call("has", {number.getValue()}) ? "have " : "not have ") << "number" 
    << std::endl;

  number = "one two three";

  std::cout << "AFTER changing number" << std::endl;
  std::cout << "List does " << 
    ((var) list.Call("has", {hello_world.getValue()}) ? "have " : "not have ") << "hello_world, and does " <<
    ((var) list.Call("has", {number.getValue()}) ? "have " : "not have ") << "number" 
    << std::endl;

  return 0;
}

// templates, list, tuple, dict, add
