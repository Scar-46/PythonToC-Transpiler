#pragma once
// Copyright (c) 2024 Syntax Errors.
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#define implicit
#define unused [[maybe_unused]]

class Object;
using ObjectPtr = std::shared_ptr<Object>;

class Object {
 public:
  virtual ~Object() = default;

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

  // Compare operators
  virtual bool equals(const Object& other) const {
    return this == &other;
  }

  virtual bool lessThan(unused const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
  }

  virtual bool greaterThan(unused const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
  }

  virtual bool lessThanOrEqual(const Object& other) const {
    return !greaterThan(other);
  }

  virtual bool greaterThanOrEqual(const Object& other) const {
    return !lessThan(other);
  }

  // Shift operations
  virtual std::shared_ptr<Object> shiftLeft(unused const Object& other) const {
    throw std::runtime_error("Shift left not supported for this type");
  }

  virtual std::shared_ptr<Object> shiftRight(unused const Object& other) const {
    throw std::runtime_error("Shift right not supported for this type");
  }

  // Methods to be implmentaed by all classes
  virtual void print(std::ostream& os) const = 0;
  virtual ObjectPtr clone() const = 0;

  virtual bool isSameType(const Object& other) const {
    return typeid(*this) == typeid(other);
  }
};



#define DEFINE_OPERATOR(OP_NAME, OP_SYMBOL, ERROR_MESSAGE) \
  ObjectPtr OP_NAME(unused const Object& other) const override { \
      auto otherObj = dynamic_cast<const Derived*>(&other); \
      if (otherObj) { \
          return std::make_shared<Derived>(value OP_SYMBOL otherObj->getValue()); \
      } else { \
          return OP_NAME##Helper(other); \
      } \
  } \
  \
  virtual ObjectPtr OP_NAME##Helper(unused const Object& other) const { \
      throw std::runtime_error(#ERROR_MESSAGE " not supported for different types"); \
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

    // Define arithmetic operators
    DEFINE_OPERATOR(add, +, 'Addition')
    // DEFINE_OPERATOR(subtract, -, 'Substraction')
};

// Boolean class
class Boolean : public BaseObject<Boolean, bool> {
 public:
  explicit Boolean(bool value) : BaseObject(value) {}
};

// Double class
class Double : public BaseObject<Double, double> {
 public:
  explicit Double(double value) : BaseObject(value) {}
};

// Integer class
class Integer : public BaseObject<Integer, int32_t> {
 public:
  explicit Integer(int32_t value) : BaseObject(value) {}
  ObjectPtr addHelper(unused const Object& other) const override {
    std::cout << "Addhelper for Integer" << std::endl;
    return std::make_shared<Integer>(100);
  }
};

// String class
class String : public BaseObject<String, std::string> {
 public:
  explicit String(std::string value) : BaseObject(std::move(value)) {}

  // Override subtract to throw an error or provide custom behavior
  ObjectPtr subtract(unused const Object& other) const override {
    throw std::runtime_error("Subtraction not supported for strings");
  }
};
