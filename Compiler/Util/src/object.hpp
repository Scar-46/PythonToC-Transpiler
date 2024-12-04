#pragma once
// Copyright (c) 2024 Syntax Errors.
#include <compare>
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

  // Conversion operator to bool (can be customized based on logic)
  explicit operator bool() const {
    throw std::runtime_error("Boolean conversion not supported for this type");
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

  explicit operator bool() const {
    return static_cast<bool>(value);
  }

  // Default implementation of clone
  inline ObjectPtr clone() const override {
    return std::make_shared<Derived>(value);
  }
};

// Boolean class
class Boolean : public BaseObject<Boolean, bool> {
 public:
  explicit Boolean(bool value) : BaseObject(value) {}

  bool equals(const Object& other) const override {
    auto otherObj = dynamic_cast<const Boolean*>(&other);
    return this->value == otherObj->getValue();
  }

  bool less(const Object& other) const override {
    auto otherObj = dynamic_cast<const Boolean*>(&other);
    return this->value < otherObj->getValue();
  }

  bool greater(const Object& other) const override {
    auto otherObj = dynamic_cast<const Boolean*>(&other);
    return this->value > otherObj->getValue();
  }
};

// String class
class String : public BaseObject<String, std::string> {
 public:
  explicit String(std::string value) : BaseObject(std::move(value)) {}

  ObjectPtr add(const Object& other) const override {
    auto otherObj = dynamic_cast<const String*>(&other);
    if (otherObj) {
      return std::make_shared<String>(value + otherObj->getValue());
    }
    throw std::runtime_error("Cannot concat non string type");
  }

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

// Base template class for numeric Object
template <typename Derived, typename ValueType>
class BaseNumeric : public Object {
 protected:
  ValueType value;

 public:
  explicit BaseNumeric(ValueType value) : value(std::move(value)) {}
  ~BaseNumeric() override = default;
  inline const ValueType& getValue() const { return value; }

  explicit operator bool() const {
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
  
  bool equals(const Object& other) const override {
    auto otherObj = dynamic_cast<const Derived*>(&other);
    if (otherObj) {
      return this->value == otherObj->getValue();
    }
    return false;
  }

  bool less(const Object& other) const override {
    auto otherObj = dynamic_cast<const Derived*>(&other);
    if (otherObj) {
      return this->value < otherObj->getValue();
    }
    return false;
  }

  bool greater(const Object& other) const override {
    auto otherObj = dynamic_cast<const Derived*>(&other);
    if (otherObj) {
      return this->value > otherObj->getValue();
    }
    return false;
  }
};

// Double class
class Double : public BaseNumeric<Double, double> {
 public:
  explicit Double(double value) : BaseNumeric(value) {}

  ObjectPtr addHelper(const Object& other) const override {
    return addWithInteger(other);
  }

  ObjectPtr subtractHelper(const Object& other) const override {
    return subtractWithInteger(other);
  }

  ObjectPtr multiplyHelper(const Object& other) const override {
    return multiplyWithInteger(other);
  }

  ObjectPtr divideHelper(const Object& other) const override {
    return divideWithInteger(other);
  }

  // ObjectPtr equalsHelper(const Object& other) const override {
  //   return equalsWithInteger(other);
  // }

  // ObjectPtr lessHelper(const Object& other) const override {
  //   return lessWithInteger(other);
  // }

  // ObjectPtr greaterHelper(const Object& other) const override {
  //   return greaterWithInteger(other);
  // }

  ObjectPtr addWithInteger(const Object& other) const;
  ObjectPtr subtractWithInteger(const Object& other) const;
  ObjectPtr multiplyWithInteger(const Object& other) const;
  ObjectPtr divideWithInteger(const Object& other) const;
  // ObjectPtr equalsWithInteger(const Object& other) const;
  // ObjectPtr lessWithInteger(const Object& other) const;
  // ObjectPtr greaterWithInteger(const Object& other) const;
};

// Integer class
// TODO(Dwayne): Implmenet like division.
class Integer : public BaseNumeric<Integer, int32_t> {
 public:
  explicit Integer(int32_t value) : BaseNumeric(value) {}

  ObjectPtr addHelper(const Object& other) const override {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
      return std::make_shared<Integer>(value + otherObj->getValue());
    }
    throw std::runtime_error("Integer does not support addition with given type");
  }

  ObjectPtr subtractHelper(const Object& other) const override {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
      return std::make_shared<Integer>(value - otherObj->getValue());
    }
    throw std::runtime_error("Integer does not support subtraction with given type");
  }

  ObjectPtr multiplyHelper(const Object& other) const override {
    auto otherObj = dynamic_cast<const Double*>(&other);
    if (otherObj) {
      return std::make_shared<Integer>(value + otherObj->getValue());
    }
    throw std::runtime_error("Integer does not support multiplication with given type");
  }

//   ObjectPtr divideHelper(const Object& other) const override {
//     auto otherObj = dynamic_cast<const Double*>(&other);
//     if (otherObj) {
//       return std::make_shared<Integer>(value + otherObj->getValue());
//     }
//     throw std::runtime_error("Integer does not support division with given type");
//   }

//   ObjectPtr equalsHelper(const Object& other) const override {
//     auto otherObj = dynamic_cast<const Double*>(&other);
//     if (otherObj) {
//       return std::make_shared<Integer>(value == otherObj->getValue());
//     }
//     throw std::runtime_error("Integer does not support comparison with given type");
//   }

//   ObjectPtr lessHelper(const Object& other) const override {
//     auto otherObj = dynamic_cast<const Double*>(&other);
//     if (otherObj) {
//       return std::make_shared<Integer>(value < otherObj->getValue());
//     }
//     throw std::runtime_error("Integer does not support comparison with given type");
//   }

//   ObjectPtr greaterHelper(const Object& other) const override {
//     auto otherObj = dynamic_cast<const Double*>(&other);
//     if (otherObj) {
//       return std::make_shared<Integer>(value > otherObj->getValue());
//     }
//     throw std::runtime_error("Integer does not support comparison with given type");
//   }
};

ObjectPtr Double::addWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return std::make_shared<Double>(value + otherObj->getValue());
  }
  throw std::runtime_error("Double does not support addition with given type");
}

ObjectPtr Double::subtractWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return std::make_shared<Double>(value - otherObj->getValue());
  }
  throw std::runtime_error("Double does not support subtraction with given type");
}

ObjectPtr Double::multiplyWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return std::make_shared<Double>(value * otherObj->getValue());
  }
  throw std::runtime_error("Double does not support multiplication with given type");
}

ObjectPtr Double::divideWithInteger(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (otherObj) {
    return std::make_shared<Double>(value / otherObj->getValue());
  }
  throw std::runtime_error("Double does not support division with given type");
}

// ObjectPtr Double::equalsWithInteger(const Object& other) const {
//   auto otherObj = dynamic_cast<const Integer*>(&other);
//   if (otherObj) {
//     return std::make_shared<Double>(value == otherObj->getValue());
//   }
//   throw std::runtime_error("Double does not support comparison with given type");
// }

// ObjectPtr Double::lessWithInteger(const Object& other) const {
//   auto otherObj = dynamic_cast<const Integer*>(&other);
//   if (otherObj) {
//     return std::make_shared<Double>(value < otherObj->getValue());
//   }
//   throw std::runtime_error("Double does not support comparison with given type");
// }

// ObjectPtr Double::greaterWithInteger(const Object& other) const {
//   auto otherObj = dynamic_cast<const Integer*>(&other);
//   if (otherObj) {
//     return std::make_shared<Double>(value > otherObj->getValue());
//   }
//   throw std::runtime_error("Double does not support comparison with given type");
// }

