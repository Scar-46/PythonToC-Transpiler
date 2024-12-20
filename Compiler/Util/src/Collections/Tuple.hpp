// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <map>
#include <utility>
#include <vector>

#include "./Object/object.hpp"
#include "./Object/var.hpp"

#include "./Collection.hpp"

class Tuple : public Collection<Tuple, std::vector> {
 private:
  void init();

 public:
  // Default constructor
  Tuple();

  // Copy-constructor
  Tuple(const Tuple& other);
  explicit Tuple(const std::vector<var>& elements);

  ~Tuple() override = default;

  // ------------------ Native overrides ------------------
  // Print contents
  void print(std::ostream& os) const override;

  // Clone self
  ObjectPtr clone() const override;

  // ------------------ Native operators ------------------
  operator ObjectPtr() override;

  bool equals(const Object& other) const override;

  bool less(const Object& other) const override;

  bool greater(const Object& other) const override;

  // Return a tuple with elements from both tuples (self, then other's)
  ObjectPtr add(const Object& other) const override;

  // Access a given element on the collection by index
  ObjectPtr subscript(const Object& other) const override;

  Method::result_type slice(const std::vector<ObjectPtr>& params);
  // ------------------ Management methods ------------------
  // Return sliced tuple
  // Method::result_type slice(const std::vector<ObjectPtr>& params);
  
  // Return index of first ocurrence of element
  Object::Method::result_type index(const std::vector<ObjectPtr>& params);

  // Return string representation of tuple
  Object::Method::result_type asString(const std::vector<ObjectPtr>& params);
};
