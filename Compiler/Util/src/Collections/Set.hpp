// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <unordered_set>
#include "../Collections/Collection.hpp"

class Set : public Collection<Set, std::unordered_set> {
  private:
    void init() {
      _methods["add"] = std::bind(&Set::add, this, std::placeholders::_1);
      _methods["union"] = std::bind(&Set::unionW, this, std::placeholders::_1);
      _methods["intersection"] = std::bind(&Set::intersectionW, this, std::placeholders::_1);
      _methods["difference"] = std::bind(&Set::differenceW, this, std::placeholders::_1);
    }

 public:
  // Default constructor
  Set() { init(); };

  // Copy-constructor
  Set(const Set& other) : Collection<Set, std::unordered_set>(other) { init(); };
  Set(const std::unordered_set<var>& elements): Collection<Set, std::unordered_set>(elements) { init(); };

  // Brace-list constructor
  Set(std::initializer_list<var> initList) : Collection<Set, std::unordered_set>(initList) { init(); }

  virtual ~Set() override = default;

  // ------------------ Native overrides ------------------

  // Print set contents
  void print(std::ostream& os) const override {
    os << "{";
    for (auto it = _elements.begin(); it != _elements.end(); ++it) {
      os << *it;
      if (std::next(it) != _elements.end()) {
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

  bool equals(const Object& other) const override {
    auto otherSet = dynamic_cast<const Set*>(&other);
    if (! otherSet) { return false; }

    return _elements == otherSet->_elements;
  }

  // ------------------ Management Methods ------------------

  // Add element to set
  Method::result_type add(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("add: Invalid number of arguments");
    }

    _elements.insert(params[0]);
    return nullptr;
  }

  // Remove specified element from set
  Method::result_type remove(const std::vector<ObjectPtr>& params) override {
    if (params.size() != 1) {
      throw std::runtime_error("remove: Invalid number of arguments");
    }

    _elements.erase(params[0]);
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

    std::unordered_set<var> result = this->_elements;
    result.insert(set->_elements.begin(), set->_elements.end());

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

    for (const var& element : this->_elements) {
      if (other->_elements.contains(element)) {
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

    for (const var& element : this->_elements) {
      if (! other->_elements.contains(element)) {
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
    explicit SetIterator(const Set& set) : _set(set), _currentIt(set._elements.begin()) {}

    bool hasNext() const override {
      return _currentIt != _set._elements.end();
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
