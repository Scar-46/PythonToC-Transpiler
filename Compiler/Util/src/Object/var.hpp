// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <compare>
#include <typeindex>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "./object.hpp"

class var;

class Iterator : public Object{
  // Underlying object iterator
  Object::ObjectIt objectIterator;

  // Whether the end has been reached or not
  bool isEnd;

 public:
  // Constructor for a valid iterator
  explicit Iterator(Object::ObjectIt iterator);

  // Constructor for the end iterator
  Iterator();

  // Copy constructor
  Iterator(const Iterator& other);

  // Object methods intialization
  void init();

  // Inherited methods from Object 
  void print(std::ostream& os) const override;
  ObjectPtr clone() const override;

  // De-referencing
  var operator*() const;
  var operator*();

  // Advancing
  Iterator& operator++();

  // Object Next Method
  Method::result_type next(const std::vector<ObjectPtr>& params);

  // Comparison
  bool operator!=(const Iterator& other) const;
};

class var {
 private:
  ObjectPtr value;

 public:
  var();
  template <typename T, typename = std::enable_if_t<std::is_base_of<Object, T>::value>>
  implicit var(const T& value) : value(std::make_shared<T>(value)) {}

  // Specialized constructors for base types
  implicit var(int32_t value);
  implicit var(double value);
  implicit var(const std::string& value);
  implicit var(const char* value);
  explicit var(bool value);

  // Copy constructor and assignment
  var(const var& other);
  var& operator=(const var& other);

  // Move constructor and assignment
  var(var&& other) noexcept;

  // Copy-assignment from ObjPtr
  implicit var(const ObjectPtr& obj);
  implicit var& operator=(const ObjectPtr& other) noexcept;

  // Access and basic conversion
  explicit operator bool() const;

  implicit operator const Object&() const;

  implicit operator Object&();

  explicit operator ObjectPtr&();

  explicit operator const ObjectPtr&();

  ObjectPtr operator->() const;

  ObjectPtr getValue() const;

  template<typename ObjectType>
  std::shared_ptr<ObjectType> as() {
    return std::dynamic_pointer_cast<ObjectType>(value);
  }

  // Comparison operators
  bool operator==(const var& other) const;

  bool operator!=(const var& other) const;

  std::strong_ordering operator<=>(const var& other) const;

  // Hashing for associative containers
  std::size_t hash() const;

  // Arithmetic operators
  var operator+(const var& other) const;

  var operator-(const var& other) const;

  var operator*(const var& other) const;

  var operator/(const var& other) const;

  var operator[](const var& other) const;

  // Print for output
  friend std::ostream& operator<<(std::ostream& os, const var& variable);

 public:
  // Provide `begin()` and `end()` methods for range-based for loops
  Iterator getIterator() const;
  
  Iterator cbegin() const;

  Iterator cend() const;

  Iterator begin();

  Iterator end();

  // Specific methods per instance
  ObjectPtr Call(const std::string& name, std::initializer_list<ObjectPtr> params);
};

// Hashing for var in associative containers
namespace std {
  template<> struct hash<var> {
    size_t operator()(const var& s) const noexcept
    { return s.hash();}
  };
}
