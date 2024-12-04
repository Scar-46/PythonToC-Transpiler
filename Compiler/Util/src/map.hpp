#pragma once
// Copyright (c) 2024 Syntax Errors.
#include "./object.hpp"
#include "./var.hpp"
#include <map>

class Map : public Object {
 private:
  std::map<var, var> elements;

 public:
  Map() = default;
  Map(std::initializer_list<std::pair<var, var>> initList) {
    for (const auto& pair : initList) {
      //elements[pair.first] = pair.second;
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

  bool equals(const Object& other) const override {
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
    //elements[key] = value;
    //TODO: var needs to have "<" operator
  }
};
