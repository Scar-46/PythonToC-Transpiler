// Copyright (c) 2024 Syntax Errors.
#pragma once

#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "../Object/object.hpp"
#include "../Object/var.hpp"

#include "../Integer/Integer.hpp"

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
    _methods["has"] = std::bind(&List::has, this, std::placeholders::_1);
    _methods["append"] = std::bind(&List::append, this, std::placeholders::_1);
    _methods["slice"] = std::bind(&List::slice, this, std::placeholders::_1);
  }

 public:
  List() : Object() { this->init(); }
  template <typename... Args>
  implicit List(Args&&... args) : elements{var(std::forward<Args>(args))...} { init(); }
  List(std::initializer_list<var> initList) : elements(initList) { init(); }
  explicit List(std::vector<var> elements) : elements(elements) { init(); }

  // ------------------ Overrides -----------------

  // Override the add method to handle list addition
  ObjectPtr add(const Object& other) const override {
    auto otherList = dynamic_cast<const List*>(&other);
     if (!otherList) {
        throw std::invalid_argument("add method requires a List");
    }
    List result = *this + *otherList;
    return std::make_shared<List>(result);
  }
  // Override the subscript method to indexation
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

  // Override the equals method to compare the lists
  bool equals(const Object& other) const override {
    auto otherList = dynamic_cast<const List*>(&other);
    if (!otherList) {
      throw std::invalid_argument("add method requires a List");
    }
    return elements == otherList->elements;
  }

  // Override print to display list contents
  void print(std::ostream& os) const override {
    os << "[";
    for (size_t i = 0; i < elements.size(); ++i) {
      os << elements[i];
      if (i < elements.size() - 1) {
        os << ", ";
      }
    }
    os << "] \n";
  }

  // Override clone to copy the list
  ObjectPtr clone() const override {
    return std::make_shared<List>(*this);
  }

  // ------------------ List Methods ------------------
  void addElement(const var& element) {
    elements.push_back(element);
  }

  var popElement(size_t index) {
    if (index >= elements.size()) {
      std::cerr << "Index out of bounds\n";
      return var();
    }
    var removedElement = elements[index];
    elements.erase(elements.begin() + index);
    return removedElement;
  }

  void clear() {
    elements.clear();
  }

  void insert(int pos, const var& element) {
    if (pos < 0 || static_cast<size_t>(pos) > elements.size()) {
      std::cerr << "Position out of bounds\n";
      return;   // Or throw an exception
    }
    elements.insert(elements.begin() + pos, element);
  }

  size_t size() const {
    return elements.size();
  }

  // ------------------ Operator Overloading ------------------
  // Overload the + operator to concatenate two lists
  List operator+(const List& other) const {
    List result = *this;  // Start with a copy of the current list
    result.elements.insert(result.elements.end(), other.elements.begin(), other.elements.end());
    return result;
  }

  // ------------------ Iterator ------------------
  class ListIterator : public Object::ObjectIterator {
   private:
    const List& _list;
    size_t _currentIndex;

   public:
    explicit ListIterator(const List& list) : _list(list), _currentIndex(0) {}

    bool hasNext() const override {
      return _currentIndex < _list.size();
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

  // ------------------ List Methods for Bind ------------------
  ObjectPtr has(std::initializer_list<ObjectPtr> params) {
    auto query = var(*(params.begin()));
    Boolean result = Boolean(false);
    if (!query) return std::make_shared<Boolean>(result);
    for (const auto& element : elements) {
      if (element == query) {
        result = Boolean(true);
      }
    }

    return std::make_shared<Boolean>(result);
  }

  ObjectPtr append(std::initializer_list<ObjectPtr> params) {
    ObjectPtr element = *(params.begin());

    elements.push_back(var(element->clone()));
    return nullptr;
  }

  ObjectPtr slice(std::initializer_list<ObjectPtr> params) {
    std::tuple<Integer, Integer, Integer> values = {Integer(0), Integer(-1), Integer(1)};
    auto it = params.begin();
    assignValues(values, it, params.end());

    int start = std::get<0>(values).getValue();
    int end = std::get<1>(values).getValue();
    int step = std::get<2>(values).getValue();

    if (step == 0) {
      throw std::invalid_argument("Step cannot be zero");
    }

    // Handle negative step (reversing the slice)
    size_t normalizedStart = normalizeIndex(start);
    size_t normalizedEnd = normalizeIndex(end);
    List result;
    if (step > 0) {
      for (size_t i = normalizedStart; i < normalizedEnd; i += step) {
        result.addElement(elements[i]);
      }
    } else {
      if (start < end) {
        throw std::invalid_argument("For negative step, start must be greater than end.");
      } else if (normalizedStart >= this->elements.size()) {
        throw std::invalid_argument("Index out of bounds.");
      }
      for (int i = normalizedStart; i >= static_cast<int>(normalizedEnd); i += step) {
        result.addElement(elements[i]);
      }
    }

    return std::make_shared<List>(result);
  }
};
