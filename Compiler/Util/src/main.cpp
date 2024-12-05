// Copyright (c) 2024 Syntax Errors.
#include <vector>

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
  var duble = 12.7;
  var boolean = "false";
  List list = List();
  var list2 = List();
  Map mapa = Map();
  var hi;

  hi = Tuple(integer, 80);

  std::cout << hi << std::endl;

  mapa.addElement(duble,boolean);

  for(auto i : hi){
    std::cout << i << std::endl;
  }

  mapa.print(std::cout);
  list.addElement("Good");
  list.addElement("4");
  list.addElement("Nite");

  var hello = list;

  std::cout << mapa[duble] <<std::endl;

  std::cout << (list.equals(list2)) <<std::endl;

  return 0;
}

// templates, list, tuple, dict, add
