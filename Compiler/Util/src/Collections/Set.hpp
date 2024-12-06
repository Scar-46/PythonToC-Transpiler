#pragma once
// Copyright (c) 2024 Syntax Errors.
#include "./Object/object.hpp"
#include "./Object/var.hpp"
#include <set>

class Set : public Object {
 private:
  std::set<var> elements;

 public:
  Set() = default;
  template <typename... Args>
  Set(Args&&... args) : elements{var(std::forward<Args>(args))...} {}
  Set(std::initializer_list<var> initList) : elements(initList) {}

  // ------------------ Overrides ------------------
  // Override the equals method to compare sets
  bool equals(const Object& other) const override {
    auto otherSet = dynamic_cast<const Set*>(&other);
    if (!otherSet) {
      throw std::invalid_argument("equals method requires a Set");
    }
    return elements == otherSet->elements;
  }

  // Override print to display set contents
  void print(std::ostream& os) const override {
    os << "{";
    for (auto it = elements.begin(); it != elements.end(); ++it) {
      os << *it;
      if (std::next(it) != elements.end()) {
        os << ", ";
      }
    }
    os << "} \n";
  }

  // Override clone to copy the set
  ObjectPtr clone() const override {
    return std::make_shared<Set>(*this);
  }

  // ------------------ Set Methods ------------------

  bool addElement(const var& element) {
    return elements.insert(element).second; // Returns true if insertion was successful (element was unique)
  }

  bool removeElement(const var& element) {
    return elements.erase(element) > 0; // Returns true if an element was removed
  }

  void clear() {
    elements.clear();
  }

  bool contains(const var& element) const {
    return elements.find(element) != elements.end();
  }

  size_t size() const {
    return elements.size();
  }

  Set unionW(const Set& other) const {
    Set result = *this;
    for (const auto& el : other.elements) {
      result.addElement(el);
    }
    return result;
  }

  Set intersectionW(const Set& other) const {
    Set result;
    for (const auto& el : elements) {
      if (other.contains(el)) {
        result.addElement(el);
      }
    }
    return result;
  }

  Set differenceW(const Set& other) const {
    Set result;
    for (const auto& el : elements) {
      if (!other.contains(el)) {
        result.addElement(el);
      }
    }
    return result;
  }

  // ------------------ Operator Overloading ------------------

  // ------------------ Iterator ------------------
  std::set<var>::iterator begin() {
    return elements.begin();
  }

  std::set<var>::iterator end() {
    return elements.end();
  }
};
