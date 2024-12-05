// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "./Object/object.hpp"
#include "./Object/var.hpp"

class Tuple : public Object {       // TODO(Dwayne): var needs to be able to get tup = {}
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
  explicit Tuple(Args&&... args) : elements{var(std::forward<Args>(args))...} {}
  Tuple(std::initializer_list<var> initList) : elements(initList) {}

  // ------------------ Overrides ------------------
  ObjectPtr add(const Object& other) const override {
    auto otherTuple = dynamic_cast<const Tuple*>(&other);
     if (!otherTuple) {
        throw std::invalid_argument("add method requires a Tuple");
    }
    Tuple result = *this + *otherTuple;
    return std::make_shared<Tuple>(result);
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

  // Overload the + operator to concatenate two tuples
  Tuple operator+(const Tuple& other) const {
    Tuple result = *this;  // Start with a copy of the current tuples
    result.elements.insert(result.elements.end(), other.elements.begin(), other.elements.end());
    return result;
  }

  // ------------------ Iterator ------------------
  class TupleIterator : public Object::ObjectIterator {
   private:
    const Tuple& _tuple;
    size_t _currentIndex;
   public:
    explicit TupleIterator(const Tuple& tuple) : _tuple(tuple), _currentIndex(0) {}

    bool hasNext() const override {
      return _currentIndex < _tuple.size();
    }

    ObjectPtr next() override {
      if (!this->hasNext()) {
        throw std::out_of_range("Iterator out of range");
    }
      return _tuple.elements[_currentIndex++].getValue();
    }

    ObjectIt clone() const override {
      return std::make_unique<TupleIterator>(*this);
    }
  };

  // Override iteration methods
  ObjectIt getIterator() const override {
    return std::make_unique<TupleIterator>(*this);
  }
};
