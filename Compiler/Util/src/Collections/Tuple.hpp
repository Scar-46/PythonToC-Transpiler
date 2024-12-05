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
  template <typename... Args>
  Tuple(Args&&... args) : elements{var(std::forward<Args>(args))...} {}
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

  // Override the subscript method to indexation
  ObjectPtr subscript(const Object& other) const override {
      // Attempt to cast the 'other' object to an Integer
      auto otherObj = dynamic_cast<const Integer*>(&other);

      if (otherObj) {
          int index = otherObj->getValue();  // Assuming 'getValue' gets the integer value of the index
          return elements[normalizeIndex(index)].operator->();
      } else {
          // Handle the case where 'other' is not an Integer (throw an exception, or return a default value)
          std::cerr << "Invalid index type, expected Integer.\n";
          return nullptr;  // Or throw an exception
      }
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
