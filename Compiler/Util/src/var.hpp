#pragma once
// Copyright (c) 2024 Syntax Errors.
#include <typeindex>

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "./object.hpp"

class List;
class var {
  ObjectPtr value;

 public:
  var() : value(nullptr) {}
  template <typename T, typename = std::enable_if_t<std::is_base_of<Object, T>::value>>
  implicit var(const T& value) : value(std::make_shared<T>(value)) {}

  // Specialized constructors for base types
  implicit var(int32_t value) : value(std::make_shared<Integer>(value)) {}
  implicit var(double value) : value(std::make_shared<Double>(value)) {}
  implicit var(const std::string& value) : value(std::make_shared<String>(value)) {}
  implicit var(const char* value) : value(std::make_shared<String>(std::string(value))) {}
  explicit var(bool value) : value(std::make_shared<Boolean>(value)) {}

  // Specialized constructor for List
  explicit var(const List& value) : value(std::static_pointer_cast<Object>(std::make_shared<List>(value))) {}

  // Copy constructor and assignment
  var(const var& other) : value(other.value ? other.value->clone() : nullptr) {}
  var& operator=(const var& other) {
    if (this != &other) {
      value = other.value ? other.value->clone() : nullptr;
    }
    return *this;
  }

  // Specialized assignment
  var& operator=(const List& list2) {
    this->value = std::static_pointer_cast<Object>(std::make_shared<List>(list2));
    return *this;
  }

  // Assignment for common types
  var& operator=(int32_t value) {
    this->value = std::make_shared<Integer>(value);
    return *this;
  }

  var& operator=(double value) {
    this->value = std::make_shared<Double>(value);
    return *this;
  }

  var& operator=(bool value) {
    this->value = std::make_shared<Boolean>(value);
    return *this;
  }

  var& operator=(const std::string& value) {
    this->value = std::make_shared<String>(value);
    return *this;
  }

  var& operator=(const char* value) {
    this->value = std::make_shared<String>(std::string(value));
    return *this;
  }

  // Move constructor and move assignment
  var(var&& other) noexcept : value(std::move(other.value)) {}
  var& operator=(var&& other) noexcept {
    if (this != &other) {
      value = std::move(other.value);
      other.value = nullptr;
    }
    return *this;
  }

  // Print for output
  friend std::ostream& operator<<(std::ostream& os, const var& variable) {
    if (variable.value) {
      variable.value->print(os);
    } else {
      os << "null";
    }
    return os;
  }
};
