// Copyright (c) 2024 Syntax Errors.
#include "Pair.hpp"

Pair::operator ObjectPtr() {
  return std::make_shared<Pair>(*this);
}

// Parameterized constructor
Pair::Pair(var first, var second)
  : value(std::make_pair(first, second)) {}

// Copy constructor
Pair::Pair(const Pair& other)
  : value(other.value) {}

// Move constructor
Pair::Pair(Pair&& other) noexcept
  : value(std::move(other.value)) {}

// Copy assignment operator
Pair& Pair::operator=(const Pair& other) {
  if (this != &other) {
    value = other.value;
  }
  return *this;
}

// Move assignment operator
Pair& Pair::operator=(Pair&& other) noexcept {
  if (this != &other) {
    value = std::move(other.value);
  }
  return *this;
}

// Equality operators
inline bool Pair::operator==(const Pair& other) const {
  return value == other.value;
}

bool Pair::equals(const Object& other) const {
  auto otherObj = dynamic_cast<const Pair*>(&other);
  if (otherObj) {
    return value == otherObj->value;
  }
  throw std::runtime_error("Pair does not support comparison with given type");
}

bool Pair::operator!=(const Pair& other) const {
  return !(*this == other);
}

// Comparison operators
bool Pair::operator<(const Pair& other) const {
  return value < other.value;
}

bool Pair::less(const Object& other) const {
  auto otherObj = dynamic_cast<const Pair*>(&other);
  if (otherObj) {
    return value < otherObj->value;
  }
  throw std::runtime_error("Pair does not support comparison with given type");
}

bool Pair::operator<=(const Pair& other) const {
  return value <= other.value;
}

bool Pair::operator>(const Pair& other) const {
  return value > other.value;
}

bool Pair::greater(const Object& other) const {
  auto otherObj = dynamic_cast<const Pair*>(&other);
  if (otherObj) {
    return value > otherObj->value;
  }
  throw std::runtime_error("Pair does not support comparison with given type");
}

bool Pair::operator>=(const Pair& other) const {
  return value >= other.value;
}

// Accessors
var Pair::getFirst() const {
  return value.first;
}

void Pair::setFirst(const var& first) {
  value.first = first;
}

var Pair::getSecond() const {
  return value.second;
}

void Pair::setSecond(const var& second) {
  value.second = second;
}

ObjectPtr Pair::clone() const {
  return std::make_shared<Pair>(*this);
}

// Swap method
void Pair::swap(Pair& other) noexcept {
  std::swap(value, other.value);
}

// Print function
void Pair::print(std::ostream& os) const {
  os << "(";
  value.first->print(os);
  os << ", ";
  value.second->print(os);
  os << ")";
}

// Non-member swap for ADL
inline void swap(Pair& lhs, Pair& rhs) noexcept {
  lhs.swap(rhs);
}
