// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "./Collection.hpp"
#include "../functions.hpp"

class List : public Collection<List, std::vector> {
 private:
  void init();

 public:
  // Default constructor
  List();

  // Copy constructor
  List(const List& other);
  explicit List(const std::vector<var>& elements);

  // Brace-list constructor
  List(std::initializer_list<var> initList);

  ~List() override = default;

  // ------------------ Native overrides -----------------
  // Print list contents
  void print(std::ostream& os) const override;

  // Clone self
  ObjectPtr clone() const override;

  // ------------------ Native operators ------------------

  operator ObjectPtr() override;

  bool equals(const Object& other) const override;

  bool less(const Object& other) const override;

  bool greater(const Object& other) const override;

  // Return a list with elements from both list (self, then other's)
  ObjectPtr add(const Object& other) const override;

  // Access a given element on the collection by index
  ObjectPtr subscript(const Object& other) const override;

  // ------------------ Management methods ------------------

  // Add element to end of list
  Method::result_type append(const std::vector<ObjectPtr>& params);

  // Insert element on given index
  Method::result_type insert(const std::vector<ObjectPtr>& params);

  // Return index of first ocurrence of element
  Method::result_type index(const std::vector<ObjectPtr>& params);

  // Return sliced list
  Method::result_type slice(const std::vector<ObjectPtr>& params);
};
