// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <memory>
#include <string>
#include <utility>

#include "../Object/object.hpp"       // NOLINT
#include "../Integer/Integer.hpp"     // NOLINT

// String class
class String : public BaseObject<String, std::string> {
 private:
  using BaseObject::value;

 public:
  explicit String(std::string value) : BaseObject(std::move(value)) {}

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

  ObjectPtr add(const Object& other) const override {
    auto otherObj = dynamic_cast<const String*>(&other);
    if (otherObj) {
      return std::make_shared<String>(value + otherObj->getValue());
    }
    throw std::runtime_error("Cannot concat non string type");
  }

  ObjectPtr subscript(const Object& other) const override {
    auto otherObj = dynamic_cast<const Integer*>(&other);
    if (otherObj) {
      auto result = value[otherObj->getValue()];
      return std::make_shared<String>(std::string(1, result));
    }
    throw std::runtime_error("Cannot Index with non integer type");
  }

  // Override iteration methods
  iterator begin() override { return value.begin(); }
  iterator end() override { return value.end(); }
  const_iterator begin() const override { return value.begin(); }
  const_iterator end() const override { return value.end(); }
  const_iterator cbegin() const override { return value.cbegin(); }
  const_iterator cend() const override { return value.cend(); }
};
