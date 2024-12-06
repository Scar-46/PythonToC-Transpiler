// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <compare>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <utility>

#include <functional>
#include <map>
#include <type_traits>

#define implicit
#define unused [[maybe_unused]]

// Forward-declarations
class Object;
using ObjectPtr = std::shared_ptr<Object>;

class Object {
 protected:
  // Callable methods by name and parameters
  using Method = std::function<ObjectPtr(std::initializer_list<ObjectPtr>)>;
  std::map<std::string, Method> _methods;

  virtual void init() {}

 public:
  virtual ~Object() = default;
  virtual operator ObjectPtr() {
    throw std::runtime_error("ObjectPtr conversion is not defined by this object");
  }

  // Conversion operator to bool (can be customized based on logic)
  virtual explicit operator bool() const {
    throw std::runtime_error("Boolean conversion not supported for this type");
  }

  // Compare operators
  virtual bool equals(unused const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
  }

  virtual bool less(unused const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
  }

  virtual bool greater(unused const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
  }

  // Arithmetic operators
  virtual ObjectPtr add(unused const Object& other) const {
    throw std::runtime_error("Addition not supported for this type");
  }

  virtual ObjectPtr subtract(unused const Object& other) const {
    throw std::runtime_error("Subtraction not supported for this type");
  }

  virtual ObjectPtr multiply(unused const Object& other) const {
    throw std::runtime_error("Multiplication not supported for this type");
  }

  virtual ObjectPtr divide(unused const Object& other) const {
    throw std::runtime_error("Division not supported for this type");
  }

  virtual ObjectPtr subscript(unused const Object& other) const {
    throw std::runtime_error("Subscript not supported for this type");
  }

  // Shift operations
  virtual std::shared_ptr<Object> shiftLeft(unused const Object& other) const {
    throw std::runtime_error("Shift left not supported for this type");
  }

  virtual std::shared_ptr<Object> shiftRight(unused const Object& other) const {
    throw std::runtime_error("Shift right not supported for this type");
  }

  // Methods to be implemented by all classes
  virtual void print(std::ostream& os) const = 0;
  virtual ObjectPtr clone() const = 0;

  virtual bool isSameType(const Object& other) const {
    return typeid(*this) == typeid(other);
  }

  // Specific methods per instance
  ObjectPtr Call(const std::string& name, std::initializer_list<ObjectPtr> params) {
    auto matchedMethod = _methods.find(name);

      if (matchedMethod == _methods.end()) {
          throw std::runtime_error("Object has no method " + name);
      }

      return matchedMethod->second(params);
  }

  // Iterators
  class ObjectIterator;
  using ObjectIt = std::unique_ptr<Object::ObjectIterator>;

  // Default iteration behavior
  class ObjectIterator {
   public:
    virtual ~ObjectIterator() = default;

    virtual bool hasNext() const = 0;
    // virtual ObjectPtr next() const = 0;
    virtual ObjectPtr next() = 0;
    virtual ObjectIt clone() const = 0;
  };

  virtual ObjectIt getIterator() const {
    throw std::runtime_error("This object does not support iteration");
  }
};

// Base template class for Object
template <typename Derived, typename ValueType>
class BaseObject : public Object {
 protected:
  ValueType value;

 public:
  explicit BaseObject(ValueType value) : value(std::move(value)) {}
  ~BaseObject() override = default;
  inline const ValueType& getValue() const { return value; }


  // Default implementation of print
  inline void print(std::ostream& os) const override {
    #ifdef DEBUG
      os << typeid(Derived).name() << ": " << value;
    #else
      os << value;
    #endif
  }

  explicit operator bool() const override {
    if constexpr (std::is_convertible_v<ValueType, bool>) {
      return static_cast<bool>(value);
    }

    throw std::runtime_error("Boolean conversion not supported for this type");
  }

  // Default implementation of clone
  inline ObjectPtr clone() const override {
    return std::make_shared<Derived>(value);
  }
};

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

// Base template class for numeric Object
template <typename Derived, typename ValueType>
class BaseNumeric : public Object {
 protected:
  ValueType value;

 public:
  explicit BaseNumeric(ValueType value) : value(std::move(value)) {}
  ~BaseNumeric() override = default;
  inline const ValueType& getValue() const { return value; }

  explicit operator bool() const override {
    return static_cast<bool>(value);
  }

  // Default implementation of print
  inline void print(std::ostream& os) const override {
    #ifdef DEBUG
      os << typeid(Derived).name() << ": " << value;
    #else
      os << value;
    #endif
  }

  // Default implementation of clone
  inline ObjectPtr clone() const override {
      return std::make_shared<Derived>(value);
  }

  DEFINE_OPERATOR(add, +, 'Addition')
  DEFINE_OPERATOR(subtract, -, 'Subtraction')
  DEFINE_OPERATOR(multiply, *, 'Multiplication')
  DEFINE_OPERATOR(divide, /, 'Division')

  DEFINE_COMPARISON_OPERATOR(equals, ==, 'Equality')
  DEFINE_COMPARISON_OPERATOR(less, <, 'Less than')
  DEFINE_COMPARISON_OPERATOR(greater, >, 'Greater than')
};

// Helper to safely extract a value or use a default
template <typename T>
T getValueOrDefault(const ObjectPtr& obj, const T& defaultValue) {
    if (auto ptr = dynamic_cast<const T*>(&*obj)) {
        return *ptr;
    }
    return defaultValue;
}

// Iterate over tuple elements and assign values from params
template <typename Tuple, typename Iterator>
void assignValues(Tuple& values, Iterator& it, Iterator end) {
    std::apply([&](auto&... args) {
        ((it != end ? args = getValueOrDefault(*it++, args) : args), ...);
    }, values);
}
