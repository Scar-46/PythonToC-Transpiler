// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "./Primitive.hpp"

// Boolean class
class Boolean : public Primitive<Boolean, bool> {
 public:
  explicit Boolean(bool value);

  operator ObjectPtr() override;

  operator bool() const override;

  void print(std::ostream& os) const override;
};
