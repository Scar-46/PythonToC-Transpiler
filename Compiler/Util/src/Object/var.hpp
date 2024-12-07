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

class Iterator: public Object{
  Object::ObjectIt objectIterator;
  bool isEnd;

 public:
  // Constructor for a valid iterator
  explicit Iterator(Object::ObjectIt iterator)
    : objectIterator(std::move(iterator)), isEnd(false) {this->init();}

  // Constructor for the end iterator
  Iterator() : objectIterator(nullptr), isEnd(true) {this->init();}

  // Copy constructor
  Iterator(const Iterator& other)
    : objectIterator(other.objectIterator), isEnd(other.isEnd) {
    this->init();
  }

  void init() override {
    _methods["next"] = std::bind(&Iterator::next, this, std::placeholders::_1);
  }

  var operator*() const;
  var operator*();
  Method::result_type next(const std::vector<ObjectPtr>& params);

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

    void print(std::ostream& os) const override {
    os << "";
  }
  ObjectPtr clone() const override {
    return nullptr;
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

  // Copy-assignment from ObjPtr
  implicit var(const ObjectPtr& obj) : value(obj) {}
  implicit var& operator=(const ObjectPtr& other) noexcept {
    value = other;
    return *this;
  }

  // Access and basic conversion
  explicit operator bool() const {
    if (!value) { return false; }
    return static_cast<bool>(*value);
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

  // Comparison operators
  bool operator==(const var& other) const {
    return value->equals(*other.value);
  }

  bool operator!=(const var& other) const {
    return !value->equals(*other.value);
  }

  std::strong_ordering operator<=>(const var& other) const {
    if (value->equals(*other.value)) return std::strong_ordering::equal;
    if (value->less(*other.value)) return std::strong_ordering::less;
    if (value->greater(*other.value)) return std::strong_ordering::greater;
    throw std::runtime_error("Failed three-way comparison");
  }

  // Hashing for associative containers
  std::size_t hash() const {
    return value->hash();
  }

  // Arithmetic operators
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

  Iterator getIterator() const {
    if (!value) {
      throw std::runtime_error("Cannot retrieve iterator from null var");
    }
    return Iterator(value->getIterator());
  }

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

  // Specific methods per instance
  ObjectPtr Call(const std::string& name, std::initializer_list<ObjectPtr> params) {
    if (!value) {
      throw std::runtime_error("Cannot call method on null var");
    }

    return value->Call(name, params);
  }
};

// Hashing for var in associative containers
namespace std {
  template<> struct hash<var> {
    size_t operator()(const var& s) const noexcept
    { return s.hash();}
  };
}

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

ObjectPtr Iterator::next(const std::vector<ObjectPtr>& params)  {
  if (isEnd || !objectIterator) {
    throw std::runtime_error("No more elements to iterate over");
  }
  var current = **this;
  ++(*this);
  return current.getValue();
}
