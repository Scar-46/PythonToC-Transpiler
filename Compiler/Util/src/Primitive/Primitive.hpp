// Copyright (c) 2024 Syntax Errors.
#pragma once

#include "../Object/object.hpp"

// Base template class for Object
template <typename Derived, typename ValueType>
class Primitive : public Object {
 protected:
  ValueType value;

 public:
  explicit Primitive(ValueType value) : value(std::move(value)) {}
  ~Primitive() override = default;
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

    auto otherObj = dynamic_cast<const Primitive<Derived, ValueType>&>(other);
    return this->value == otherObj.getValue();
  }

  virtual bool less(const Object& other) const {
    if (!isSameType(other)) { return false ;}

    auto otherObj = dynamic_cast<const Primitive<Derived, ValueType>&>(other);
    return this->value < otherObj.getValue();
  }

  virtual bool greater(const Object& other) const {
    if (!isSameType(other)) { return false ;}

    auto otherObj = dynamic_cast<const Primitive<Derived, ValueType>&>(other);
    return this->value > otherObj.getValue();
  }  
};
