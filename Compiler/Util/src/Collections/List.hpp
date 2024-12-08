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
  std::vector<var> elements;
  void init();

 public:
  // ------------------ Constructors and destructor -----------------

  // Default constructor
  List();
  // Copy constructor
  List(const List& other);
  explicit List(const std::vector<var>& elements);
  // Destructor
  virtual ~List() override;

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
  ObjectPtr add(const Object& other) const override;
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
  // Get string representation of set
  Method::result_type asString(const std::vector<ObjectPtr>& params);
};
