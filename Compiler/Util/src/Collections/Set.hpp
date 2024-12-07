// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <unordered_set>
#include <vector>

#include "../Collections/Collection.hpp"

class Set : public Collection<Set, std::unordered_set> {
 private:
  void init();

 public:
  // Default constructor
  Set();

  // Copy-constructor
  Set(const Set& other);
  explicit Set(const std::unordered_set<var>& elements);

  // Brace-list constructor
  Set(std::initializer_list<var> initList);

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
  Object::Method::result_type add(const std::vector<ObjectPtr>& params);

  // Remove specified element from set
  Object::Method::result_type remove(const std::vector<ObjectPtr>& params);

  // Return union of self and another set
  Object::Method::result_type unionW(const std::vector<ObjectPtr>& params);

  // Return intersection of self and another set
  Object::Method::result_type intersectionW(const std::vector<ObjectPtr>& params);

  // Difference between this set (lhs) and another set (rhs)
  Object::Method::result_type differenceW(const std::vector<ObjectPtr>& params);
};
