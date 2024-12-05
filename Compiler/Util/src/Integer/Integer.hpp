// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <memory>

#include "../Object/object.hpp"
#include "../Double/Double.hpp"

// Integer class
// TODO(Dwayne): Implmenet like division.
class Integer : public BaseNumeric<Integer, int32_t> {
 public:
  explicit Integer(int32_t value) : BaseNumeric(value) {}

  ObjectPtr addHelper(const Object& other) const override {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
      return std::make_shared<Integer>(value + otherObj->getValue());
    }
    throw std::runtime_error("Integer does not support addition with given type");
  }

  ObjectPtr subtractHelper(const Object& other) const override {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
      return std::make_shared<Integer>(value - otherObj->getValue());
    }
    throw std::runtime_error("Integer does not support subtraction with given type");
  }

  ObjectPtr multiplyHelper(const Object& other) const override {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
      return std::make_shared<Integer>(value + otherObj->getValue());
    }
    throw std::runtime_error("Integer does not support multiplication with given type");
  }

  ObjectPtr divideHelper(const Object& other) const override {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
      return std::make_shared<Integer>(value + otherObj->getValue());
    }
    throw std::runtime_error("Integer does not support division with given type");
  }

  bool equalsHelper(const Object& other) const override {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
      return value == otherObj->getValue();
    }
    throw std::runtime_error("Integer does not support comparison with given type");
  }

  bool lessHelper(const Object& other) const override {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
      return value < otherObj->getValue();
    }
    throw std::runtime_error("Integer does not support comparison with given type");
  }

  bool greaterHelper(const Object& other) const override {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
      return value > otherObj->getValue();
    }
    throw std::runtime_error("Integer does not support comparison with given type");
  }
};