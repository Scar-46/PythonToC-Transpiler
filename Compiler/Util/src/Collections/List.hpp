// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "../Collections/Collection.hpp"
#include "../functions.hpp"

class List : public Collection<List, std::vector> {
 private:

  void init() {
    _methods["append"] = std::bind(&List::append, this, std::placeholders::_1);
    _methods["insert"] = std::bind(&List::insert, this, std::placeholders::_1);
    _methods["index"] = std::bind(&List::index, this, std::placeholders::_1);
    _methods["slice"] = std::bind(&List::slice, this, std::placeholders::_1);
  }

 public:
  // Default constructor
  List() { this->init(); }
  
  // Copy constructor
  List(const List& other) : Collection<List, std::vector>(other) { this->init(); }
  explicit List(const std::vector<var>& elements) : Collection<List, std::vector>(elements) { init(); }
  
  // Brace-list constructor
  List(std::initializer_list<var> initList) : Collection<List, std::vector>(initList) { init(); }

  virtual ~List() override = default;

  // ------------------ Native overrides -----------------

  // Print list contents
  void print(std::ostream& os) const override {
    os << "[";
    for (size_t i = 0; i < _elements.size(); ++i) {
      os << _elements[i];
      if (i < _elements.size() - 1) {
        os << ", ";
      }
    }
    os << "]";
  }

  // Clone self
  ObjectPtr clone() const override {
    return std::make_shared<List>(*this);
  }

  // ------------------ Native operators ------------------

  operator ObjectPtr() override {
    return std::make_shared<List>(*this);
  };

  // Concatenate iself and another list
  List operator+(const List& other) const {
    List result = *this;  // Start with a copy of the current list
    result._elements.insert(result._elements.end(), other._elements.begin(), other._elements.end());
    return result;
  }

  // Add itself and another list
  ObjectPtr add(const Object& other) const override {
    auto otherList = dynamic_cast<const List*>(&other);
     if (!otherList) {
        throw std::invalid_argument("add method requires a List");
    }
    List result = *this + *otherList;
    return std::make_shared<List>(result);
  }

  // Index-access element in list
  ObjectPtr subscript(const Object& other) const override {
    auto otherObj = dynamic_cast<const Integer*>(&other);

    if (! otherObj) {
      std::cerr << "Invalid index type, expected Integer.\n";
      return nullptr;
    }

    std::size_t index = normalizeIndex(otherObj->getValue());

    if (static_cast<size_t>(index) >= _elements.size()) {
      std::cerr << "Invalid index, out of bounds.\n";
      return nullptr; 
    }

    return _elements[index].operator->();
  }

  // ------------------ Management methods ------------------

  // Add element to end of list
  Method::result_type append(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("append: Invalid number of arguments");
    }

    ObjectPtr element = params[0];
    if (element) _elements.push_back(element->clone());

    return nullptr;
  }

  // Insert element on given index
  Method::result_type insert(const std::vector<ObjectPtr>& params) {
    if (params.size() != 2) {
      throw std::runtime_error("insert: Invalid number of arguments");
    }

    if (! params[0].get()) { 
      std::cerr << "insert: Invalid null index"; 
      return nullptr;
    }

    const Integer* pos = dynamic_cast<const Integer*>(params[0].get());

    if (! pos ) {
      std::cerr << "insert: Non-integer index"; 
      return nullptr;
    }

    std::size_t index = normalizeIndex(pos->getValue());
    var element = params[1];

    _elements.insert(_elements.begin() + index, element);
    return nullptr;
  }

  // Return index of first ocurrence of element
  Method::result_type index(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("index: Invalid number of arguments");
    }

    var query = params[0];
    for (size_t i = 0; i < _elements.size(); ++i) {
      if (_elements[i] == query) {
        return std::make_shared<Integer>(i);
      }
    }

    std::cerr << "index: Value missing from list\n"; 
    return nullptr;
  }

  // Return sliced list
  Method::result_type slice(const std::vector<ObjectPtr>& params) {
    return generalizedSlice(
      this->_elements,
      params,
      [](std::vector<var>& result, const var& element) { result.push_back(element); },
      [](const std::vector<var>& resultContainer) {
        return std::make_shared<List>(resultContainer);
      }
    );    // NOLINT
  }

  // ------------------ Iterator ------------------

  class ListIterator : public Object::ObjectIterator {
   private:
    const List& _list;
    size_t _currentIndex;

   public:
    explicit ListIterator(const List& list) : _list(list), _currentIndex(0) {}

    bool hasNext() const override {
      return _currentIndex < _list._elements.size();
    }

    ObjectPtr next() override {
      if (!this->hasNext()) {
        throw std::out_of_range("Iterator out of range");
    }
      return _list._elements[_currentIndex++].getValue();
    }

    ObjectIt clone() const override {
      return std::make_unique<ListIterator>(*this);
    }
  };

  // Override iteration methods
  ObjectIt getIterator() const override {
    return std::make_unique<ListIterator>(*this);
  }
};
