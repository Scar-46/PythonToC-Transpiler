// Copyright (c) 2024 Syntax Errors.
#include <vector>

#include "./util.hpp"

void checkReference(unused const Object& op1, unused const Object& op2) {
  std::cout << "Can be passed as object references" << std::endl;
}

void checkPointer(unused const ObjectPtr& op1, unused const ObjectPtr& op2) {
  std::cout << "Can be passed as object pointers" << std::endl;
}

// Dummy class
class List : public Object {
 public:
  List() = default;

  void print(std::ostream& os) const override {
    os << "List" << std::endl;
  }
  ObjectPtr clone() const override {
    return std::make_shared<List>();
  }

  void addChild(const var& child) {
    std::cout << "Added child: " << child << std::endl;
  }
};

int main() {
  var string = "Hello world";
  var integer = 123;
  var duble = 12.7;
  var boolean = false;
  var list = List();

  auto listPtr = list.as<List>();
  listPtr->addChild("Hello world");


  std::vector<var> variables = std::vector<var>({string, integer, duble, boolean});

  for (auto variable : variables) {
    std::cout << variable << std::endl;
  }

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
  if (one) {
    std::cout << "Can check for value" << std::endl;
  }
  std::cout << "'" << string[Integer(0)] << "' is the first letter of '" << string << "'" << std::endl;

  for (auto& item : string) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
  return 0;
}

// templates, list, tuple, dict, add
