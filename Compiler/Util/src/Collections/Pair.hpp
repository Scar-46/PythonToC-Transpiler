// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "../Object/object.hpp"
#include "../Object/var.hpp"

class Pair : public Object {
 private:
  std::pair<var, var> value;
  void init();

 public:
  // Default constructor
  Pair();

  operator ObjectPtr() override;

  // Parameterized constructor
  Pair(var first, var second);

  // Copy constructor
  Pair(const Pair& other);

  // Move constructor
  Pair(Pair&& other) noexcept;

  // Destructor
  ~Pair() noexcept override = default;

  // Copy assignment operator
  Pair& operator=(const Pair& other);

  // Move assignment operator
  Pair& operator=(Pair&& other) noexcept;

  // Equality operators
  inline bool operator==(const Pair& other) const;

  bool equals(const Object& other) const;

  bool operator!=(const Pair& other) const;

  // Comparison operators
  bool operator<(const Pair& other) const;

  bool less(const Object& other) const override;

  bool greater(const Object& other) const override;

  bool operator<=(const Pair& other) const;

  bool operator>(const Pair& other) const;

  bool operator>=(const Pair& other) const;

  // Accessors
  var getFirst() const;

  void setFirst(const var& first);

  var getSecond() const;

  void setSecond(const var& second);

  ObjectPtr clone() const override;

  // Swap method
  void swap(Pair& other) noexcept;

  // Print function
  void print(std::ostream& os) const override;

  // Management methods
  Object::Method::result_type len(const std::vector<ObjectPtr>& params);
  Object::Method::result_type asString(const std::vector<ObjectPtr>& params);
  Object::Method::result_type asBoolean(const std::vector<ObjectPtr>& params);
};

// Non-member swap for ADL
inline void swap(Pair& lhs, Pair& rhs) noexcept;
