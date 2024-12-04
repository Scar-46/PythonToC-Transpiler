// Copyright (c) 2024 Syntax Errors.
#include <vector>

#include "./util.hpp"

int main() {
  var string = "Hello world";
  var integer = 123;
  var integer2 = 5;
  var duble = 12.7;
  var boolean = false;
  List list1;
  List list2;

  list2 = {1, 2.5, 3, "hola", 5, "mundo"};

  List list3;

  list1.addElement(string);
  list1.addElement(integer2);

  list3 = list1 + list2;
  list3.print(std::cout);  // Output: [10, 20]
}

// templates, list, tuple, dict, add
