#pragma once
// Copyright (c) 2024 Syntax Errors.
#include "./Object/object.hpp"
#include "./Object/var.hpp"
#include <unordered_set>

class Set : public Object {
 private:
  std::unordered_set<var> elements;

  void init() override {
    _methods["add"] = std::bind(&Set::add, this, std::placeholders::_1);
    _methods["remove"] = std::bind(&Set::remove, this, std::placeholders::_1);
    _methods["pop"] = std::bind(&Set::pop, this, std::placeholders::_1);
    _methods["clear"] = std::bind(&Set::clear, this, std::placeholders::_1);
    _methods["union"] = std::bind(&Set::unionW, this, std::placeholders::_1);
    _methods["intersection"] = std::bind(&Set::intersectionW, this, std::placeholders::_1);
    _methods["difference"] = std::bind(&Set::differenceW, this, std::placeholders::_1);
  }

 public:
  Set() { this->init(); };
  Set(const std::unordered_set<var>& set): elements(set) { this->init(); };
  Set(const Set& other) : Object(other), elements(other.elements) { this->init(); };

  // ------------------ Native overrides ------------------

  // Print set contents
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

  // Clone self
  ObjectPtr clone() const override {
    return std::make_shared<Set>(*this);
  }

  // ------------------ Native operators ------------------

  operator ObjectPtr() override{
    return std::make_shared<Set>(*this);
  };

  // Check for equal contents between self and another set
  bool equals(const Object& other) const override {
    auto otherSet = dynamic_cast<const Set*>(&other);
    if (!otherSet) {
      throw std::invalid_argument("equals method requires a Set");
    }
    return elements == otherSet->elements;
  }

  // ------------------ Management Methods ------------------

  // Add element to set
  Method::result_type add(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("add: Invalid number of arguments");
    }

    elements.insert(params[0]);
    return nullptr;
  }

  // Remove specified element from set
  Method::result_type remove(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("remove: Invalid number of arguments");
    }

    elements.erase(params[0]);
    return nullptr;
  }

  // Remove any element from set
  Method::result_type pop(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("pop: Invalid number of arguments");
    }

    if (! elements.empty()) {
      elements.erase(elements.begin());
    }

    return nullptr;
  }

  // Remove all elements from set
  Method::result_type clear(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("clear: Invalid number of arguments");
    }

    elements.clear();
    return nullptr;
  }

  // Return union of self and another set
  Method::result_type unionW(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("union: Invalid number of arguments");
    }

    const Set* set = dynamic_cast<const Set*>(params[0].get());

    if (! set ) {
      std::cerr << "union: Parameter must be Set"; 
      return nullptr;
    }

    std::unordered_set<var> result = this->elements;
    result.insert(set->elements.begin(), set->elements.end());

    return std::make_shared<Set>(result);
  }

  // Return intersection of self and another set
  Method::result_type intersectionW(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("intersection: Invalid number of arguments");
    }

    const Set* other = dynamic_cast<const Set*>(params[0].get());

    if (! other ) {
      std::cerr << "intersection: Parameter must be Set"; 
      return nullptr;
    }

    std::unordered_set<var> result;

    for (const var& element : this->elements) {
      if (other->elements.contains(element)) {
        result.insert(element);
      }
    }

    return std::make_shared<Set>(result);
  }

  // Difference between this set (lhs) and another set (rhs)
  Method::result_type differenceW(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("difference: Invalid number of arguments");
    }

    const Set* other = dynamic_cast<const Set*>(params[0].get());

    if (! other ) {
      std::cerr << "difference: Parameter must be Set"; 
      return nullptr;
    }

    std::unordered_set<var> result;

    for (const var& element : this->elements) {
      if (! other->elements.contains(element)) {
        result.insert(element);
      }
    }

    return std::make_shared<Set>(result);
  }

  // ------------------ Iterator ------------------

  class SetIterator : public Object::ObjectIterator {
   private:
    const Set& _set;
    std::unordered_set<var>::const_iterator _currentIt;

   public:
    explicit SetIterator(const Set& set) : _set(set), _currentIt(set.elements.begin()) {}

    bool hasNext() const override {
      return _currentIt != _set.elements.end();
    }

    ObjectPtr next() override {
      if (!this->hasNext()) {
        throw std::out_of_range("Iterator out of range");
      }

      var obj = *_currentIt;
      _currentIt = std::next(_currentIt);

      return obj.getValue();
    }

    ObjectIt clone() const override {
      return std::make_unique<SetIterator>(*this);
    }
  };

  // Override iteration methods
  ObjectIt getIterator() const override {
    return std::make_unique<SetIterator>(*this);
  }
};
