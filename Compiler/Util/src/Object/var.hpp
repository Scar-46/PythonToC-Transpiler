#pragma once

// Copyright (c) 2024 Syntax Errors.
#include <compare>
#include <typeindex>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "./object.hpp"
#include "./Boolean/Boolean.hpp"
#include "./Double/Double.hpp"
#include "./Integer/Integer.hpp"
#include "./String/String.hpp"

class var;

class Iterator {
  Object::ObjectIt objectIterator;
  bool isEnd;

 public:
  // Constructor for a valid iterator
  explicit Iterator(Object::ObjectIt iterator)
    : objectIterator(std::move(iterator)), isEnd(false) {}

  // Constructor for the end iterator
  Iterator() : objectIterator(nullptr), isEnd(true) {}

  var operator*() const;
  var operator*();

  Iterator& operator++() {
    if (!objectIterator || isEnd) {
      throw std::runtime_error("Incrementing an invalid iterator");
    }
    if (!objectIterator->hasNext()) {
      isEnd = true;
    }
    return *this;
  }

  bool operator!=(const Iterator& other) const {
    return isEnd != other.isEnd;  // Simple end-check comparison
  }
};

class var {
 private:
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
  explicit var(ObjectPtr obj) : value(std::move(obj)) {}

  // Copy constructor and assignment
  var(const var& other) : value(other.value ? other.value->clone() : nullptr) {}
  var& operator=(const var& other) {
    if (this != &other) {
      value = other.value ? other.value->clone() : nullptr;
    }
    return *this;
  }

  // Move constructor and assignment
  var(var&& other) noexcept : value(std::move(other.value)) {}
  var& operator=(var&& other) noexcept {
    if (this != &other) {
      value = std::move(other.value);
      other.value = nullptr;
    }
    return *this;
  }

  // Access and basic conversion
  explicit operator bool() const {
    return static_cast<bool>(value);
  }

  implicit operator const Object&() const {
    if (!value) {
      throw std::runtime_error("Cannot convert null var to Object&");
    }
    return *value;
  }

  implicit operator Object&() {
    if (!value) {
      throw std::runtime_error("Cannot convert null var to Object&");
    }
    return *value;
  }

  explicit operator ObjectPtr&() {
    return value;
  }

  explicit operator const ObjectPtr&() {
    return value;
  }

  ObjectPtr operator->() const {
    return value;
  }

  // ObjectPtr operator->() const {
  //   return value;
  // }

  inline ObjectPtr getValue() const {
    return this->value;
  }

  template<typename ObjectType>
  std::shared_ptr<ObjectType> as() {
    return std::dynamic_pointer_cast<ObjectType>(value);
  }

  std::strong_ordering operator<=>(const var& other) const {
    if (value->equals(*other.value)) return std::strong_ordering::equal;
    if (value->less(*other.value)) return std::strong_ordering::less;
    if (value->greater(*other.value)) return std::strong_ordering::greater;
    throw std::runtime_error("Invalid comparison");
  }

  var operator==(const var& other) const {
    return value->equals(*other.value);
  }

  var operator!=(const var& other) const {
    return !value->equals(*other.value);
  }

  var operator+(const var& other) const {
    if (!value || !other.value) {
      throw std::runtime_error("Addition not supported for null values");
    }
    return var(value->add(*other.value));
  }

  var operator-(const var& other) const {
    if (!value || !other.value) {
      throw std::runtime_error("Substraction not supported for null values");
    }
    return var(value->subtract(*other.value));
  }

  var operator*(const var& other) const {
    if (!value || !other.value) {
      throw std::runtime_error("Multiplication not supported for null values");
    }
    return var(value->multiply(*other.value));
  }

  var operator/(const var& other) const {
    if (!value || !other.value) {
      throw std::runtime_error("Division not supported for null values");
    }
    return var(value->divide(*other.value));
  }

  var operator[](const var& other) const {
    if (!value || !other.value) {
      throw std::runtime_error("Subscript not supported for null values");
    }
    return var(value->subscript(*other.value));
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

 public:
  // Provide `begin()` and `end()` methods for range-based for loops
  Iterator cbegin() const {
    if (!value) {
      throw std::runtime_error("Cannot iterate over null var");
    }
    return Iterator(value->getIterator());
  }

  Iterator cend() const {
    return Iterator();
  }

  Iterator begin() {
    if (!value) {
      throw std::runtime_error("Cannot iterate over null var");
    }
    return Iterator(value->getIterator());
  }

  Iterator end() {
    return Iterator();
  }
};

var Iterator::operator*() const {
  if (!objectIterator || isEnd || !objectIterator->hasNext()) {
    throw std::runtime_error("Dereferencing an invalid iterator");
  }
  return var(objectIterator->next());
}

var Iterator::operator*() {
  if (!objectIterator || isEnd || !objectIterator->hasNext()) {
    throw std::runtime_error("Dereferencing an invalid iterator");
  }
  return var(objectIterator->next());
}
