// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "./Object/object.hpp"
#include "./Object/var.hpp"

class Tuple : public Collection<Tuple, std::vector> {       // TODO(Dwayne): var needs to be able to get tup = {}
 private:
  std::vector<var> elements;

  void init() {
    _methods.erase("pop");
    _methods.erase("clear");
    _methods.erase("remove");
    _methods["index"] = std::bind(&Tuple::index, this, std::placeholders::_1);
  }

 public:
  // Default constructor
  Tuple() { init(); };

  // Copy-constructor
  Tuple(const Tuple& other) : Collection<Tuple, std::vector>(other) { init(); };
  explicit Tuple(const std::vector<var>& elements) : Collection<Tuple, std::vector>(elements) { init(); }

  // Brace-list constructor
  Tuple(std::initializer_list<var> initList) : elements(initList) { init(); }

  virtual ~Tuple() override = default;

  // ------------------ Native overrides ------------------

  // Print contents
  void print(std::ostream& os) const override {
    os << "(";
    for (size_t i = 0; i < elements.size(); ++i) {
      os << elements[i];
      if (i < elements.size() - 1) {
        os << ", ";
      }
    }
    os << ")";
  }

  // Clone self
  ObjectPtr clone() const override {
    return std::make_shared<Tuple>(*this);
  }

  // ------------------ Native operators ------------------

  operator ObjectPtr() override {
    return std::make_shared<Tuple>(*this);
  };

  // Return a tuple with elements from both tuples (self, then other's)
  ObjectPtr add(const Object& other) const override {
    auto otherTuple = dynamic_cast<const Tuple*>(&other);

     if (!otherTuple) {
        std::cerr << "Invalid argument type, expected Tuple.\n";
        return nullptr;
    }
  
    std::vector<var> result = this->elements;
    result.insert(
      result.end(), otherTuple->elements.begin(), otherTuple->elements.end()
    );

    return std::make_shared<Tuple>(result);
  }

  // Access a given element on the tuple
  ObjectPtr subscript(const Object& other) const override {
    auto otherObj = dynamic_cast<const Integer*>(&other);

    if (! otherObj) {
      std::cerr << "Invalid index type, expected Integer.\n";
      return nullptr; 
    }

    std::size_t index = normalizeIndex(otherObj->getValue());

    if (static_cast<size_t>(index) >= elements.size()) {
      std::cerr << "Invalid index, out of bounds.\n";
      return nullptr; 
    }

    return elements[index].getValue();
  }

  // ------------------ Management methods ------------------

  // Return index of first ocurrence of element
  Method::result_type index(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("index: Invalid number of arguments");
    }

    var query = params[0];
    for (size_t i = 0; i < elements.size(); ++i) {
      if (elements[i] == query) {
        return std::make_shared<Integer>(i);
      }
    }

    std::cerr << "index: Value missing from list\n"; 
    return nullptr;
  }

  // ------------------ Iterator ------------------

  class TupleIterator : public Object::ObjectIterator {
   private:
    const Tuple& _tuple;
    size_t _currentIndex;
   public:
    explicit TupleIterator(const Tuple& tuple) : _tuple(tuple), _currentIndex(0) {}

    bool hasNext() const override {
      return _currentIndex < _tuple.elements.size();
    }

    ObjectPtr next() override {
      if (!this->hasNext()) {
        throw std::out_of_range("Iterator out of range");
      }

      return _tuple.elements[_currentIndex++].getValue();
    }

    ObjectIt clone() const override {
      return std::make_unique<TupleIterator>(*this);
    }
  };

  ObjectIt getIterator() const override {
    return std::make_unique<TupleIterator>(*this);
  }
};
