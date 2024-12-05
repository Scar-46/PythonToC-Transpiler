// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "../Object/object.hpp"

// Boolean class
class Boolean : public BaseObject<Boolean, bool> {
 public:
  explicit Boolean(bool value) : BaseObject(value) {}

  bool equals(const Object& other) const override {
    auto otherObj = dynamic_cast<const Boolean*>(&other);
    return this->value == otherObj->getValue();
  }

  bool less(const Object& other) const override {
    auto otherObj = dynamic_cast<const Boolean*>(&other);
    return this->value < otherObj->getValue();
  }

  bool greater(const Object& other) const override {
    auto otherObj = dynamic_cast<const Boolean*>(&other);
    return this->value > otherObj->getValue();
  }
};
