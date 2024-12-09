// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "./Numeric.hpp"

// Double precision floating point numbers
class Double : public Numeric<Double, double> {
 public:
  explicit Double(double value) : Numeric(value) {}
  operator ObjectPtr() override{
    return std::make_shared<Double>(*this);
  };

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

  bool equalsHelper(const Object& other) const override {
    return equalsWithInteger(other);
  }

  bool lessHelper(const Object& other) const override {
    return lessWithInteger(other);
  }

  bool greaterHelper(const Object& other) const override {
    return greaterWithInteger(other);
  }

  ObjectPtr addWithInteger(const Object& other) const;
  ObjectPtr subtractWithInteger(const Object& other) const;
  ObjectPtr multiplyWithInteger(const Object& other) const;
  ObjectPtr divideWithInteger(const Object& other) const;

  bool equalsWithInteger(const Object& other) const;
  bool lessWithInteger(const Object& other) const;
  bool greaterWithInteger(const Object& other) const;
};
