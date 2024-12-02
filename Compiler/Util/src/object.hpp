#pragma once
// Copyright (c) 2024 Syntax Errors.
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#define implicit

class Object;
using ObjectPtr = std::shared_ptr<Object>;

class Object {
 public:
  virtual ~Object() = default;

  // Arithmetic operators
  virtual ObjectPtr add(const Object& other) const {
    throw std::runtime_error("Addition not supported for this type");
  }

  virtual ObjectPtr subtract(const Object& other) const {
    throw std::runtime_error("Subtraction not supported for this type");
  }

  virtual ObjectPtr multiply(const Object& other) const {
    throw std::runtime_error("Multiplication not supported for this type");
  }

  virtual ObjectPtr divide(const Object& other) const {
    throw std::runtime_error("Division not supported for this type");
  }

  // Compare operators
  virtual bool equals(const Object& other) const {
    return this == &other;
  }

  virtual bool lessThan(const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
  }

  virtual bool greaterThan(const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
  }

  virtual bool lessThanOrEqual(const Object& other) const {
    return !greaterThan(other);
  }

  virtual bool greaterThanOrEqual(const Object& other) const {
    return !lessThan(other);
  }

  // Shift operations
  virtual std::shared_ptr<Object> shiftLeft(const Object& other) const {
    throw std::runtime_error("Shift left not supported for this type");
  }

  virtual std::shared_ptr<Object> shiftRight(const Object& other) const {
    throw std::runtime_error("Shift right not supported for this type");
  }

  // Methods to be implmentaed by all classes
  virtual void print(std::ostream& os) const = 0;
  virtual ObjectPtr clone() const = 0;

  virtual bool isSaqmeType(const Object& other) const {
    return typeid(*this) == typeid(other);
  }
};

#define DEFINE_OPERATOR(Operation, OpSymbol, MethodName)                \
  std::shared_ptr<Object> MethodName(const Object& other) const override { \
    auto otherObj = dynamic_cast<const Derived*>(&other);             \
    if (otherObj) {                                                    \
      return std::make_shared<Derived>(value OpSymbol otherObj->value); \
    }                                                                   \
    return MethodNameHelper(other);                                     \
  }                                                                     \
  virtual std::shared_ptr<Object> MethodNameHelper(const Object& other) const { \
    throw std::runtime_error(#Operation " not supported for different types"); \
  }

// Base template class for Object
template <typename Derived, typename ValueType>
class BaseObject : public Object {
 protected:
    ValueType value;

 public:
    explicit BaseObject(ValueType value) : value(std::move(value)) {}
    ~BaseObject() override = default;


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
    DEFINE_OPERATOR(Addition, +, add)
    // DEFINE_OPERATOR(Subtraction, -, subtract)
    // DEFINE_OPERATOR(Multiplication, *, multiply)
    // DEFINE_OPERATOR(Division, /, divide)

    // Define comparison operators
    // DEFINE_OPERATOR(Equal, ==, equal)
    // DEFINE_OPERATOR(LessThan, <, lessThan)
    // DEFINE_OPERATOR(GreaterThan, >, greaterThan)
    // DEFINE_OPERATOR(LessThanOrEqual, <=, lessThanOrEqual)
    // DEFINE_OPERATOR(GreaterThan, >=, greaterThanOrEqual)

    // Shift operations
    // DEFINE_OPERATOR(ShiftLeft, <<,  shiftLeft)
    // DEFINE_OPERATOR(ShiftRight, >>, shiftRight)
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

  std::shared_ptr<Object> addHelper(const Object& other) const override {
    if (auto otherInt = dynamic_cast<const Integer*>(&other)) {
      return std::make_shared<Integer>(value + otherInt->value);
    }
    if (auto otherDouble = dynamic_cast<const Double*>(&other)) {
      return std::make_shared<Double>(value + otherDouble->value);
    }
    throw std::runtime_error("Addition not supported between Integer and given type");
  }

  std::shared_ptr<Object> addHelper(const Object& other) const override {
    if (auto otherInt = dynamic_cast<const Integer*>(&other)) {
      return std::make_shared<Integer>(value + otherInt->value);
    }
    if (auto otherDouble = dynamic_cast<const Double*>(&other)) {
      return std::make_shared<Double>(value + otherDouble->value);
    }
    throw std::runtime_error("Addition not supported between Integer and given type");
  }

  // std::shared_ptr<Object> susbtractHelper(const Object& other) const override {
  //   if (auto otherInt = dynamic_cast<const Integer*>(&other)) {
  //     return std::make_shared<Integer>(value - otherInt->value);
  //   }
  //   if (auto otherDouble = dynamic_cast<const Double*>(&other)) {
  //     return std::make_shared<Double>(value - otherDouble->value);
  //   }
  //   throw std::runtime_error("Substraction not supported between Integer and given type");
  // }

  // std::shared_ptr<Object> multiplyHelper(const Object& other) const override {
  //   if (auto otherInt = dynamic_cast<const Integer*>(&other)) {
  //     return std::make_shared<Integer>(value * otherInt->value);
  //   }
  //   if (auto otherDouble = dynamic_cast<const Double*>(&other)) {
  //     return std::make_shared<Double>(value * otherDouble->value);
  //   }
  //   throw std::runtime_error("Multipliation not supported between Integer and given type");
  // }

  // std::shared_ptr<Object> divideHelper(const Object& other) const override {
  //   if (auto otherInt = dynamic_cast<const Integer*>(&other)) {
  //     return std::make_shared<Integer>(value / otherInt->value);
  //   }
  //   if (auto otherDouble = dynamic_cast<const Double*>(&other)) {
  //     return std::make_shared<Double>(value / otherDouble->value);
  //   }
  //   throw std::runtime_error("Division not supported between Integer and given type");
  // }

  // std::shared_ptr<Object> equalHelper(const Object& other) const override {
  //   if (auto otherInt = dynamic_cast<const Integer*>(&other)) {
  //     return std::make_shared<Integer>(value == otherInt->value);
  //   }
  //   if (auto otherDouble = dynamic_cast<const Double*>(&other)) {
  //     return std::make_shared<Double>(value == otherDouble->value);
  //   }
  //   throw std::runtime_error("Division not supported between Integer and given type");
  // }
};

// String class
class String : public BaseObject<String, std::string> {
 public:
  explicit String(std::string value) : BaseObject(std::move(value)) {}
};
