#pragma once
// Copyright (c) 2024 Syntax Errors.
#include "./object.hpp"
#include "./var.hpp"
#include <unordered_set>

class Set : public Object {
 private:
  std::unordered_set<var> elements;

 public:
  Set() = default;

  // ------------------ Overrides ------------------
  // Override the add method
  ObjectPtr add(const Object& other) const override {
  }

  // Override the subtract method
  ObjectPtr subtract(const Object& other) const override {

  }

  // Override the multiply method
  ObjectPtr multiply(const Object& other) const override {
  }

  // Override the divide method
  ObjectPtr divide(const Object& other) const override {

  }
  
  // Override the equals method
  bool equals(const Object& other) const override {
  }

  // Override print to display set contents
  void print(std::ostream& os) const override {
    os << "{";
    auto it = elements.begin();
    if (it != elements.end()) {
      os << *it;
      ++it;
    }
    for (; it != elements.end(); ++it) {
      os << ", " << *it;
    }
    os << "} \n";
  }

  // Override clone to copy the set
  ObjectPtr clone() const override {
  }

  // ------------------ Set Methods ------------------
  void addElement(const var& element) {
    //elements.insert(element);
  }
};
