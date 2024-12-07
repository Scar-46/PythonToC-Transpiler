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
  using Method = std::function<ObjectPtr(const std::vector<ObjectPtr>&)>;
  std::map<std::string, Method> _methods;

  virtual void init() {}

 public:
  virtual ~Object() = default;
  virtual operator ObjectPtr() {
    throw std::runtime_error("ObjectPtr conversion is not defined by this object");
  }

  // ------------------ Native operators ------------------

  // Conversion operator to bool (can be customized based on logic)
  virtual explicit operator bool() const {
    throw std::runtime_error("Boolean conversion not supported for this type");
  }

  // Conversion to hash (for associative containers)
  virtual std::size_t hash() const {
    throw std::runtime_error("Comparison not supported for this type");
  }

  // Comparison operators
  virtual bool equals(unused const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
  }

  virtual bool less(unused const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
  }

  virtual bool greater(unused const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
  }

  // Arithmetic operations
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
  virtual ObjectPtr shiftLeft(unused const Object& other) const {
    throw std::runtime_error("Shift left not supported for this type");
  }

  virtual ObjectPtr shiftRight(unused const Object& other) const {
    throw std::runtime_error("Shift right not supported for this type");
  }

  // ------------------ Native methods ------------------

  // Print contents
  virtual void print(std::ostream& os) const = 0;
  
  // Clone itself
  virtual ObjectPtr clone() const = 0;

  // Check type equivalence
  virtual bool isSameType(const Object& other) const {
    return typeid(*this) == typeid(other);
  }

  // ------------------ Per-instance methods ------------------

  // Call method supported by object instance
  ObjectPtr Call(const std::string& name, std::initializer_list<ObjectPtr> params) {
    auto matchedMethod = _methods.find(name);

      if (matchedMethod == _methods.end()) {
          throw std::runtime_error("Object has no method " + name);
      }

      return matchedMethod->second(params);
  }

  // ------------------ Iterator ------------------

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

// Hashing for associative containers
namespace std {
  template<> struct hash<Object> {
    size_t operator()(const Object& s) const noexcept
    { return s.hash();}
  };
}

// Base template class for Object
template <typename Derived, typename ValueType>
class BaseObject : public Object {
 protected:
  ValueType value;

 public:
  explicit BaseObject(ValueType value) : value(std::move(value)) {}
  ~BaseObject() override = default;
  inline const ValueType& getValue() const { return value; }

  // ------------------ Native methods ------------------

  // Print inner value contents
  inline void print(std::ostream& os) const override {
    #ifdef DEBUG
      os << typeid(Derived).name() << ": " << value;
    #else
      os << value;
    #endif
  }

  // Clone self
  inline ObjectPtr clone() const override {
    return std::make_shared<Derived>(value);
  }

  // ------------------ Native operators ------------------

  // Cast inner value into bool
  explicit operator bool() const override {
    if constexpr (std::is_convertible_v<ValueType, bool>) {
      return static_cast<bool>(value);
    }

    throw std::runtime_error("Boolean conversion not supported for this type");
  }

  // Hash inner value
  virtual std::size_t hash() const {
    return std::hash<ValueType>{}(value);
  }

  virtual bool equals(const Object& other) const {
    if (!isSameType(other)) { return false ;}

    auto otherObj = dynamic_cast<const BaseObject<Derived, ValueType>&>(other);
    return this->value == otherObj.getValue();
  }

  virtual bool less(const Object& other) const {
    if (!isSameType(other)) { return false ;}

    auto otherObj = dynamic_cast<const BaseObject<Derived, ValueType>&>(other);
    return this->value < otherObj.getValue();
  }

  virtual bool greater(const Object& other) const {
    if (!isSameType(other)) { return false ;}

    auto otherObj = dynamic_cast<const BaseObject<Derived, ValueType>&>(other);
    return this->value > otherObj.getValue();
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
