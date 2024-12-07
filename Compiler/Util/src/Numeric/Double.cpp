// Copyright (c) 2024 Syntax Errors.

#include "./Double.hpp"
#include "./Integer.hpp"

ObjectPtr Double::addWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return std::make_shared<Double>(value + otherObj->getValue());
  }
  throw std::runtime_error("Double does not support addition with given type");
}

ObjectPtr Double::subtractWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return std::make_shared<Double>(value - otherObj->getValue());
  }
  throw std::runtime_error("Double does not support subtraction with given type");
}

ObjectPtr Double::multiplyWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return std::make_shared<Double>(value * otherObj->getValue());
  }
  throw std::runtime_error("Double does not support multiplication with given type");
}

ObjectPtr Double::divideWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return std::make_shared<Double>(value / otherObj->getValue());
  }
  throw std::runtime_error("Double does not support division with given type");
}

bool Double::equalsWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return value == otherObj->getValue();
  }
  return false;
}

bool Double::lessWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return value < otherObj->getValue();
  }
  return false;
}

bool Double::greaterWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return value > otherObj->getValue();
  }
  return false;
}