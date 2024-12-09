// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <unordered_set>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include "../Collections/Collection.hpp"

class Set : public Collection<Set, std::unordered_set> {
 private:
  void init();

 public:
  // Default constructor
  Set();

  // Copy-constructor
  Set(const Set& other);
  implicit Set(const std::unordered_set<var>& elements);

  ~Set() override = default;

  // ------------------ Native overrides ------------------

  // Print set contents
  void print(std::ostream& os) const override;

  // Clone self
  ObjectPtr clone() const override;

    // ------------------ Native operators ------------------

  operator ObjectPtr() override;

  bool equals(const Object& other) const override;

  // ------------------ Management Methods ------------------
  // Add element to set
  Method::result_type add(const std::vector<ObjectPtr>& params);

  // Lookup an element in set
  Method::result_type has(const std::vector<ObjectPtr>& params);

  // Remove specified element from set
  Method::result_type remove(const std::vector<ObjectPtr>& params) override;

  // Return union of self and another set
  Method::result_type unionW(const std::vector<ObjectPtr>& params);

  // Return intersection of self and another set
  Method::result_type intersectionW(const std::vector<ObjectPtr>& params);

  // Difference between this set (lhs) and another set (rhs)
  Method::result_type differenceW(const std::vector<ObjectPtr>& params);

  // Get string representation of set
  Method::result_type asString(const std::vector<ObjectPtr>& params);
};
