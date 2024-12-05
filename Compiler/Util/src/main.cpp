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
  var floating = 12.7;
  var boolean = false;
  var list = List();
  var list2 = List();
  var mapa = Map();
  var hi;

  hi = Tuple(integer, 80);

  std::cout << "Tuple: " << hi << std::endl;

  std::cout << "Adding pair: " << var(Pair(floating, boolean)) << std::endl;
  mapa.as<Map>()->addElement(integer, string);
  mapa.as<Map>()->addElement(floating, boolean);
  var access = mapa[floating];
  std::cout << "Accessed[" << floating << "] : " << access << std::endl;
  var keys = var(mapa.as<Map>()->keys());
  var values = var(mapa.as<Map>()->values());
  var items = var(mapa.as<Map>()->items());
  std::cout << "Map: " << mapa << " | Keys: " << keys  << " | Values: " << values << " | Iems: " << items << std::endl;

  list.as<List>()->addElement("Good");
  list.as<List>()->addElement("4");
  list.as<List>()->addElement("Nite");

  var hello = list;

  list->print(std::cout);
  std::cout << " == ";
  hello->print(std::cout);
  std::cout << "?: " << (list == hello) <<std::endl;

  std::cout << "List: ";
  for (const auto& it : list) {
    std::cout << it << ", ";
  }
  std::cout << std::endl;

  std::cout << "Map: ";
  for (const auto& it : mapa) {
    std::cout << it << std::endl;
  }
  std::cout << std::endl;

  integer = "I do not want to do this";
  std::cout << integer << std::endl;

  var one = 1;
  var twoPointTree = 2.3;
  var result = one - twoPointTree;
  var second_one = 1;
  std::cout << "Result: " << result << std::endl;
  std::cout << one << " != " << second_one << "?: " << (one != second_one) << std::endl;

  if (one < twoPointTree) {
    std::cout << "True" << std::endl;
  }
  var newString = "New string";

  std::cout << "'" << newString << "' is ";
  if (newString < integer) {
    std::cout << "less";
  } else if (newString > integer) {
    std::cout << "greater";
  } else if (newString <= integer) {
    std::cout << "less or equal";
  } else if (newString >= integer) {
    std::cout << "greater or equal";
  } else if (newString == integer) {
    std::cout << "Equal";
  }
  std::cout << " than '" << integer << "'" << std::endl;

  if (newString == "New string") {
    std::cout << "Can compare strings to const char*" << std::endl;
  }

  checkReference(one, one);
  checkPointer(static_cast<ObjectPtr&>(one), static_cast<ObjectPtr&>(one));
  if (one && twoPointTree) {
    std::cout << "Can check for value" << std::endl;
  }
  std::cout << "'" << string[Integer(0)] << "' is the first letter of '" << string << "'" << std::endl;

  one = 1;
  var two = 2;
  var hola = "hola";
  var four = 4;
  var five = 5;
  List set = {one, two, hola, four, five};
  if (set.has(Integer(2))) {
    std::cout << "2 in ";
    set.print(std::cout);
  }

  return 0;
}

// templates, list, tuple, dict, add
