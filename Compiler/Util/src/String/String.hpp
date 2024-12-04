// Copyright (c) 2024 Syntax Errors.
#pragma once
#include "../Object/object.hpp"     // NOLINT

// String class
class String : public BaseObject<String, std::string> {
 public:
  explicit String(std::string value) : BaseObject(std::move(value)) {}

  ObjectPtr add(const Object& other) const override {
    auto otherObj = dynamic_cast<const String*>(&other);
    if (otherObj) {
      return std::make_shared<String>(value + otherObj->getValue());
    }
    throw std::runtime_error("Cannot concat non string type");
  }

  bool equals(const Object& other) const override {
    auto otherObj = dynamic_cast<const String*>(&other);
    return this->value == otherObj->getValue();
  }

  bool less(const Object& other) const override {
    auto otherObj = dynamic_cast<const String*>(&other);
    return this->value < otherObj->getValue();
  }

  bool greater(const Object& other) const override {
    auto otherObj = dynamic_cast<const String*>(&other);
    return this->value > otherObj->getValue();
  }
};
