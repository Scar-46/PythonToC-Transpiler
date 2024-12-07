// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "../Object/object.hpp"
#include "../Object/var.hpp"
#include "../Integer/Integer.hpp"

#include "../functions.hpp"

class List : public Object {
 private:
  std::vector<var> elements;

  size_t normalizeIndex(int index) const {
    if (index < 0) {
      index += (elements.size());
    }
    if (index < 0 || static_cast<size_t>(index) > elements.size()) {
      throw std::out_of_range("Index out of bounds");
    }
    return static_cast<size_t>(index);
  }

  void init() override {
    _methods["append"] = std::bind(&List::append, this, std::placeholders::_1);
    _methods["insert"] = std::bind(&List::insert, this, std::placeholders::_1);
    _methods["clear"] = std::bind(&List::clear, this, std::placeholders::_1);
    _methods["has"] = std::bind(&List::has, this, std::placeholders::_1);
    _methods["slice"] = std::bind(&List::slice, this, std::placeholders::_1);
    _methods["size"] = std::bind(&List::size, this, std::placeholders::_1);
  }

 public:
  List() : Object() { this->init(); }
  
  // Copy constructor
  List(const List& other) : Object(other), elements(other.elements) {
      this->init();  // Initialize methods for the new object
  }
  template <typename... Args>
  implicit List(Args&&... args) : elements{var(std::forward<Args>(args))...} { init(); }
  List(std::initializer_list<var> initList) : elements(initList) { init(); }
  explicit List(std::vector<var> elements) : elements(elements) { init(); }

  // ------------------ Native overrides -----------------

  // Print list contents
  void print(std::ostream& os) const override {
    os << "[";
    for (size_t i = 0; i < elements.size(); ++i) {
      os << elements[i];
      if (i < elements.size() - 1) {
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

  // Concatenate iself and another list
  List operator+(const List& other) const {
    List result = *this;  // Start with a copy of the current list
    result.elements.insert(result.elements.end(), other.elements.begin(), other.elements.end());
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
    // Attempt to cast the 'other' object to an Integer
    auto otherObj = dynamic_cast<const Integer*>(&other);

    if (otherObj) {
      int index = otherObj->getValue();  // Assuming 'getValue' gets the integer value of the index
      return elements[normalizeIndex(index)].operator->();
    } else {
      // Handle the case where 'other' is not an Integer (throw an exception, or return a default value)
      std::cerr << "Invalid index type, expected Integer.\n";
      return nullptr;  // Or throw an exception
    }
  }

  // Compare contents of self with another list
  bool equals(const Object& other) const override {
    auto otherList = dynamic_cast<const List*>(&other);
    if (!otherList) {
      throw std::invalid_argument("add method requires a List");
    }
    return elements == otherList->elements;
  }

  // ------------------ Management methods ------------------

  // Add element to end of list
  Method::result_type append(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("append: Invalid number of arguments");
    }

    ObjectPtr element = params[0];
    if (element) elements.push_back(element->clone());

    return nullptr;
  }

  // Remove all elements in list
  Method::result_type clear(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("clear: Invalid number of arguments");
    }

    elements.clear();
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

    var element = params[1];

    if (pos->getValue() < 0 || pos->getValue() > elements.size()) {
      std::cerr << "Position out of bounds\n";
      return nullptr;
    }

    elements.insert(pos->getValue() + elements.begin(), element);
    return nullptr;
  }

  // Get amount of elements
  Method::result_type size(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
      throw std::runtime_error("size: Invalid number of arguments");
    }

    return std::make_shared<Integer>(elements.size());
  }

  // Scan for element
  Method::result_type has(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("has: Invalid number of arguments");
    }

    ObjectPtr queryPtr = params[0];
    if (!queryPtr) return std::make_shared<Boolean>(false);

    auto query = var(queryPtr);
    for (const auto& element : elements) {
      if (element == query) {
        return std::make_shared<Boolean>(true);
      }
    }

    return std::make_shared<Boolean>(false);
  }

  // Return sliced list
  Method::result_type slice(const std::vector<ObjectPtr>& params) {
    return generalizedSlice(
      this->elements,
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
      return _currentIndex < _list.elements.size();
    }

    ObjectPtr next() override {
      if (!this->hasNext()) {
        throw std::out_of_range("Iterator out of range");
    }
      return _list.elements[_currentIndex++].getValue();
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
