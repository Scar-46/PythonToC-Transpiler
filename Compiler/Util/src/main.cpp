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
  var boolean = false;
  List list = List();
  List list2 = List();
  Map mapa = Map();
  
  mapa.addElement(duble,boolean);



  mapa.print(std::cout);
  list.addElement("Good");
  list.addElement("4");
  list.addElement("Nite");

  std::cout << (list.equals(list2)) <<std::endl;

  for(auto i : list){
    std::cout << i << std::endl;
  }
 

  // for (auto variable : list) {
  //   std::cout << variable << std::endl;
  // }

  // integer = "I do not want to do this";
  // std::cout << integer << std::endl;

  // var one = 1;
  // var twoPointTree = 2.3;
  // var result = one - twoPointTree;
  // var second_one = 1;
  // std::cout << "Result: " << result << std::endl;
  // std::cout << one << " != " << second_one << "?: " << (one != second_one) << std::endl;

  // if (one < twoPointTree) {
  //   std::cout << "True" << std::endl;
  // }
  // var newString = "New string";

  // std::cout << "'" << newString << "' is ";
  // if (newString < integer) {
  //   std::cout << "less";
  // } else if (newString > integer) {
  //   std::cout << "greater";
  // } else if (newString <= integer) {
  //   std::cout << "less or equal";
  // } else if (newString >= integer) {
  //   std::cout << "greater or equal";
  // } else if (newString == integer) {
  //   std::cout << "Equal";
  // }
  // std::cout << " than '" << integer << "'" << std::endl;

  // if (newString == "New string") {
  //   std::cout << "Can compare strings to const char*" << std::endl;
  // }

  // checkReference(one, one);
  // checkPointer(static_cast<ObjectPtr&>(one), static_cast<ObjectPtr&>(one));
  // if (one) {
  //   std::cout << "Can check for value" << std::endl;
  // }
  // std::cout << "'" << string[Integer(0)] << "' is the first letter of '" << string << "'" << std::endl;
  return 0;
  // std::cout << integer + duble << std::endl;
}

// templates, list, tuple, dict, add
