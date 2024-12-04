#pragma once
// Copyright (c) 2024 Syntax Errors.
#include "./Object/object.hpp"
#include "./Object/var.hpp"

class Tuple : public Object { //TODO():var needs to be able to get tup = {}
 private:
  std::vector<var> elements;

  size_t normalizeIndex(int index) const {
    if (index < 0) {
      index += (elements.size());
    }
    if (index < 0 || static_cast<size_t>(index) >= elements.size()) {
      throw std::out_of_range("Index out of bounds");
    }
    return static_cast<size_t>(index);
  }

 public:
  Tuple() = default;
  Tuple(std::initializer_list<var> initList) : elements(initList) {}

  // ------------------ Overrides ------------------
  // Override the add method to handle tuple addition (unsupported for tuples)
  ObjectPtr add(const Object& other) const override {
    throw std::invalid_argument("add method is not supported for Tuple");
  }

  // Override the subtract method to handle tuple subtraction (unsupported for tuples)
  ObjectPtr subtract(const Object& other) const override {
    throw std::invalid_argument("subtract method is not supported for Tuple");
  }

  // Override the multiply method to handle tuple multiplication (unsupported for tuples)
  ObjectPtr multiply(const Object& other) const override {
    throw std::invalid_argument("multiply method is not supported for Tuple");
  }

  // Override the divide method to handle tuple division (unsupported for tuples)
  ObjectPtr divide(const Object& other) const override {
    throw std::invalid_argument("divide method is not supported for Tuple");
  }

  // Override the equals method to compare tuples
  bool equals(const Object& other) const override {
    auto otherTuple = dynamic_cast<const Tuple*>(&other);
    if (!otherTuple) {
      return false;
    }
    return elements == otherTuple->elements;
  }

  // Override print to display tuple contents
  void print(std::ostream& os) const override {
    os << "(";
    for (size_t i = 0; i < elements.size(); ++i) {
      os << elements[i];
      if (i < elements.size() - 1) {
        os << ", ";
      }
    }
    os << ") \n";
  }

  // Override clone to copy the tuple
  ObjectPtr clone() const override {
    return std::make_shared<Tuple>(*this);
  }

  // ------------------ Tuple Methods ------------------
  var getElement(size_t index) const {
    if (index >= elements.size()) {
      std::cerr << "Index out of bounds\n";
      return var();
    }
    return elements[index];
  }

  size_t size() const {
    return elements.size();
  }

  // ------------------ Operator Overloading ------------------

  // Overload the [] operator to access elements by index
  var operator[](int index) const {
    return elements[normalizeIndex(index)];
  }

  // ------------------ Iterator ------------------
  std::vector<var>::const_iterator begin() const {
    return elements.begin();
  }

  std::vector<var>::const_iterator end() const {
    return elements.end();
  }
};
