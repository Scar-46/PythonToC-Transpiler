// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <memory>
#include <utility>

#include "../Object/object.hpp"
#include "../Object/var.hpp"


class Pair : public Object {
 private:
  std::pair<var, var> value;

 public:
  // Default constructor
  Pair() : value() {}

  operator ObjectPtr() override{
    return std::make_shared<Pair>(*this);
  };

  // Parameterized constructor
  Pair(var first, var second) : value(std::make_pair(first, second)) {}

  // Copy constructor
  Pair(const Pair& other) : value(other.value) {}

  // Move constructor
  Pair(Pair&& other) noexcept : value(std::move(other.value)) {}

  // Destructor
  ~Pair() noexcept override = default;

  // Copy assignment operator
  Pair& operator=(const Pair& other) {
    if (this != &other) {
      value = other.value;
    }
    return *this;
  }

  // Move assignment operator
  Pair& operator=(Pair&& other) noexcept {
    if (this != &other) {
      value = std::move(other.value);
    }
    return *this;
  }

  // Equality operators
  inline bool operator==(const Pair& other) const {
    return value == other.value;
  }

  bool equals(const Object& other) const {
    auto otherObj = dynamic_cast<const Pair*>(&other);
    if (otherObj) {
      return value == otherObj->value;
    }
    throw std::runtime_error("Pair does not support comparison with given type");
  }

  bool operator!=(const Pair& other) const {
    return !(*this == other);
  }

  // Comparison operators
  bool operator<(const Pair& other) const {
    return value < other.value;
  }

  bool less(const Object& other) const override {
    auto otherObj = dynamic_cast<const Pair*>(&other);
    if (otherObj) {
      return value < otherObj->value;
    }
    throw std::runtime_error("Pair does not support comparison with given type");
  }

  bool operator<=(const Pair& other) const {
    return value <= other.value;
  }

  bool operator>(const Pair& other) const {
    return value > other.value;
  }

  bool greater(const Object& other) const override {
    auto otherObj = dynamic_cast<const Pair*>(&other);
    if (otherObj) {
      return value > otherObj->value;
    }
    throw std::runtime_error("Pair does not support comparison with given type");
  }

  bool operator>=(const Pair& other) const {
    return value >= other.value;
  }

  // Accessors
  var getFirst() const {
    return value.first;
  }

  void setFirst(const var& first) {
    value.first = first;
  }

  var getSecond() const {
    return value.second;
  }

  void setSecond(const var& second) {
    value.second = second;
  }

  ObjectPtr clone() const override {
    return std::make_shared<Pair>(*this);
  }

  // Swap method
  void swap(Pair& other) noexcept {
    std::swap(value, other.value);
  }

  // Print function
  void print(std::ostream& os) const override {
    os << "(";
    value.first->print(os);
    os << ", ";
    value.second->print(os);
    os << ")";
  }

};

// Non-member swap for ADL
inline void swap(Pair& lhs, Pair& rhs) noexcept {
  lhs.swap(rhs);
}
