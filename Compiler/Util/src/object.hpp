#pragma once
// Copyright (c) 2024 Syntax Errors.
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <utility>
#include <type_traits>

#include "./factory.hpp"

#define implicit

class Object;
using ObjectPtr = std::shared_ptr<Object>;

class Object {
 public:
  virtual ~Object() = default;

  // Methods to be implmentaed by all classes
  virtual void print(std::ostream& os) const = 0;
  virtual ObjectPtr clone() const = 0;
};

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
};

// Double class
class Double : public BaseObject<Double, double> {
 public:
  explicit Double(double value) : BaseObject(value) {}
};

// Integer class
class Integer : public BaseObject<Integer, int> {
 public:
  explicit Integer(int value) : BaseObject(value) {}
};

// String class
class String : public BaseObject<String, std::string> {
 public:
  explicit String(std::string value) : BaseObject(std::move(value)) {}
};
