#pragma once
// Copyright (c) 2024 Syntax Errors.
#include "./Object/object.hpp"
#include "./Object/var.hpp"
#include <map>

class Map : public Object {
 private:
  std::map<var, var> elements;

 public:
  Map() = default;
  Map(std::initializer_list<std::pair<var, var>> initList) {
    for (const auto& pair : initList) {
      elements[pair.first] = pair.second;
    }
}

  // ------------------ Overrides ------------------
  ObjectPtr add(const Object& other) const override {
    // Implement map addition logic
  }

  ObjectPtr subtract(const Object& other) const override {
    // Implement map subtraction logic
  }

  ObjectPtr multiply(const Object& other) const override {
    // Implement map multiplication logic
  }

  ObjectPtr divide(const Object& other) const override {
    // Implement map division logic
  }

  // Override the subscript method to implement indexation
  ObjectPtr subscript(const Object& other) const override {
    const var* otherObj = dynamic_cast<const var*>(&other);

    if (otherObj) {
      var index = *otherObj;
      auto it = elements.find(index);

      if (it != elements.end()) {
        return it->second.operator->();
      } else {
        std::cerr << "Key not found\n";
        return nullptr;  // Or throw an exception if needed
      }
    } else {
      // Handle the case where 'other' is not a 'var' (or appropriate type)
      std::cerr << "Invalid index type, expected 'var' (key type).\n";
      return nullptr;  // Or throw an exception
    }
  }

  bool equals(const Object& other) const override {
    auto otherMap  = dynamic_cast<const Map*>(&other);
    if (!otherMap) {
      throw std::invalid_argument("add method requires a List");
    }
    return elements == otherMap->elements;
  }

  void print(std::ostream& os) const override {
    os << "{";
    for (auto it = elements.begin(); it != elements.end(); ++it) {
      os << it->first << ": " << it->second;
      if (std::next(it) != elements.end()) {
        os << ", ";
      }
    }
    os << "} \n";
  }

  ObjectPtr clone() const override {
    return std::make_shared<Map>(*this);
  }

  // ------------------ Map Methods ------------------
  void addElement(const var& key, const var& value) {
    elements[key] = value;
  }

  var popElement(const var& key) {
    auto it = elements.find(key);
    if (it != elements.end()) {
      var removedElement = it->second;
      elements.erase(it);
      return removedElement;
    } else {
      std::cerr << "Key not found\n";
      return var();
    }
  }

  void clear() {
    elements.clear();
  }

  size_t size() const {
    return elements.size();
  }

  // ------------------ Operator Overloading ------------------
  // Overload the [] operator to access elements by key
  var operator[](const var& key) const {
    auto it = elements.find(key);
    if (it != elements.end()) {
      return it->second;
    } else {
      std::cerr << "Key not found\n";
      return var();  // Or throw an exception
    }
  }

  // Overload the + operator to merge two maps
  Map operator+(const Map& other) const {
    Map result = *this; // Start with a copy of the current map
    for (const auto& pair : other.elements) {
      result.elements[pair.first] = pair.second;  // Overwrite or insert new key-value pairs
    }
    return result;
  }

  // ------------------ Iterator ------------------
  std::map<var, var>::iterator begin() {
    return elements.begin();
  }

  std::map<var, var>::iterator end() {
    return elements.end();
  }
};
