// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "../Object/object.hpp"

// Double class
class Double : public BaseNumeric<Double, double> {
 public:
  explicit Double(double value) : BaseNumeric(value) {}

  ObjectPtr addHelper(const Object& other) const override {
    return addWithInteger(other);
  }

  ObjectPtr subtractHelper(const Object& other) const override {
    return subtractWithInteger(other);
  }

  ObjectPtr multiplyHelper(const Object& other) const override {
    return multiplyWithInteger(other);
  }

  ObjectPtr divideHelper(const Object& other) const override {
    return divideWithInteger(other);
  }

  bool equalsHelper(const Object& other) const override {
    return equalsWithInteger(other);
  }

  bool lessHelper(const Object& other) const override {
    return lessWithInteger(other);
  }

  bool greaterHelper(const Object& other) const override {
    return greaterWithInteger(other);
  }

  ObjectPtr addWithInteger(const Object& other) const;
  ObjectPtr subtractWithInteger(const Object& other) const;
  ObjectPtr multiplyWithInteger(const Object& other) const;
  ObjectPtr divideWithInteger(const Object& other) const;

  bool equalsWithInteger(const Object& other) const;
  bool lessWithInteger(const Object& other) const;
  bool greaterWithInteger(const Object& other) const;
};

#include "../Integer/Integer.hpp"
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
  throw std::runtime_error("Double does not support comparison with given type");
}

bool Double::lessWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return value < otherObj->getValue();
  }
  throw std::runtime_error("Double does not support comparison with given type");
}

bool Double::greaterWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return value > otherObj->getValue();
  }
  throw std::runtime_error("Double does not support comparison with given type");
}