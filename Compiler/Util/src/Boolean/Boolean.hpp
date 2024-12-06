// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <memory>
#include "../Object/object.hpp"

// Boolean class
class Boolean : public BaseObject<Boolean, bool> {
 private:
  // using BaseObject::BaseObject;

 public:
  explicit Boolean(bool value) : BaseObject(value) {}

  operator ObjectPtr() override{
    return std::make_shared<Boolean>(*this);
  };

  void print(std::ostream& os) const override {
    os << ((this->value)? "True" : "False");
  }

  operator bool() const override { return this->value; }
};
