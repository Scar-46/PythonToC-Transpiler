// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <unordered_map>
#include <memory>
#include <utility>
#include <vector>
#include <list>
#include "../Object/object.hpp"
#include "../Object/var.hpp"
#include "../Primitive/Boolean.hpp"
#include "./Pair.hpp"
#include "./List.hpp"

class Map : public Object {
 private:
  std::unordered_map<var, var> elements;

  void init();

 public:
  Map();
  Map(const Map& other);
  Map(std::initializer_list<std::pair<var, var>> initList);

  template <typename... Args>
  explicit Map(Args&&... args) {
    if constexpr (sizeof...(args) > 0) {
      (addPair(std::forward<Args>(args)), ...);
    }
    this->init();
  }

  // ------------------ Native overrides ------------------

  // Override the addition method to implement map addition
  ObjectPtr add(unused const Object& other) const override;
  // Get value from associated key-value pair
  ObjectPtr subscript(const Object& other) const override;
  // Test equality with other maps
  bool equals(const Object& other) const override;
  // Print contents
  void print(std::ostream& os) const override;
  // Clone itself
  ObjectPtr clone() const override;

  // ------------------ Native operators ------------------

  // Access elements in key-value pairs by key
  var operator[](const var& key) const;
  // Return merge of itself and another map
  Map operator+(const Map& other) const;

  // ------------------ Management Methods ------------------

  // Add key-value entry 
  Method::result_type addElement(const std::vector<ObjectPtr>& params);
  // Remove key-value entry by given key
  Method::result_type pop(const std::vector<ObjectPtr>& params);
  // Remove all key-value entries
  Method::result_type clear(const std::vector<ObjectPtr>& params);
  // Returns the size of the map
  size_t size() const;
  // Returns a list of all values in the map
  Method::result_type keys(const std::vector<ObjectPtr>& params);
  // Returns a list of all values in the map
  Method::result_type values(const std::vector<ObjectPtr>& params);
  // Returns a list of key-value pairs as Pair objects
  Method::result_type items(const std::vector<ObjectPtr>& params);
  // Get value associated with key-value pair by key
  Method::result_type get(const std::vector<ObjectPtr>& params);
  // Amount of key-value entries in the map
  Method::result_type len(const std::vector<ObjectPtr>& params);
  // Smallest key in map
  Method::result_type min(const std::vector<ObjectPtr>& params);
  // Greatest key in map
  Method::result_type max(const std::vector<ObjectPtr>& params);
  // Sum of all keys in the map
  Method::result_type sum(const std::vector<ObjectPtr>& params);
  // True if any items remain in the map
  Method::result_type asBoolean(const std::vector<ObjectPtr>& params);
  // String representation of map
  Method::result_type asString(const std::vector<ObjectPtr>& params);

  // ------------------ Iterator ------------------
  class MapIterator : public Object::ObjectIterator {
   private:
    const Map& _map;
    size_t _currentIndex;

   public:
    explicit MapIterator(const Map& list);
    bool hasNext() const override;
    ObjectPtr next() override;
    ObjectIt clone() const override;
  };

  // Override iteration methods
  ObjectIt getIterator() const override;
};
