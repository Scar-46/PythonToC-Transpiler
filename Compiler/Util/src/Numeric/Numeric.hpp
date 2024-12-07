// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <compare>
#include <iostream>

#include "../Object/object.hpp"

#define implicit
#define unused [[maybe_unused]]

// Arithmetic operation helper macro
#define DEFINE_OPERATOR(OP_NAME, OP_SYMBOL, ERROR_MESSAGE) \
  ObjectPtr OP_NAME(const Object& other) const override { \
    auto otherObj = dynamic_cast<const Derived*>(&other); \
    if (otherObj) { \
      return std::make_shared<Derived>(value OP_SYMBOL otherObj->getValue()); \
    } \
    return OP_NAME##Helper(other); \
  } \
  \
  virtual ObjectPtr OP_NAME##Helper(unused const Object& other) const { \
      throw std::runtime_error(#ERROR_MESSAGE " not supported for different types"); \
  }

// Comparison operation helper macro
#define DEFINE_COMPARISON_OPERATOR(OP_NAME, OP_SYMBOL, ERROR_MESSAGE) \
  bool OP_NAME(const Object& other) const override { \
    auto otherObj = dynamic_cast<const Derived*>(&other); \
    if (otherObj) { \
      return value OP_SYMBOL otherObj->getValue(); \
    } \
    return OP_NAME##Helper(other); \
  } \
  \
  virtual bool OP_NAME##Helper(unused const Object& other) const { \
      return false; \
  }

// Base template class for numeric objects
template <typename Derived, typename ValueType>
class BaseNumeric : public Object {
 protected:
  ValueType value;

 public:
  explicit BaseNumeric(ValueType value) : value(std::move(value)) {}
  ~BaseNumeric() override = default;
  inline const ValueType& getValue() const { return value; }

  // ------------------ Native methods ------------------

  // Print inner number
  inline void print(std::ostream& os) const override {
    #ifdef DEBUG
      os << typeid(Derived).name() << ": " << value;
    #else
      os << value;
    #endif
  }

  inline ObjectPtr clone() const override {
      return std::make_shared<Derived>(value);
  }

  // ------------------ Native operators ------------------

  // Cast inner number into bool
  explicit operator bool() const override {
    return static_cast<bool>(value);
  }

  // Hash inner number
  virtual std::size_t hash() const {
    return std::hash<ValueType>{}(value);
  }

  // Arithmethic between numbers
  DEFINE_OPERATOR(add, +, 'Addition')
  DEFINE_OPERATOR(subtract, -, 'Subtraction')
  DEFINE_OPERATOR(multiply, *, 'Multiplication')
  DEFINE_OPERATOR(divide, /, 'Division')

  // Comparison between numbers
  DEFINE_COMPARISON_OPERATOR(equals, ==, 'Equality')
  DEFINE_COMPARISON_OPERATOR(less, <, 'Less than')
  DEFINE_COMPARISON_OPERATOR(greater, >, 'Greater than')
};
