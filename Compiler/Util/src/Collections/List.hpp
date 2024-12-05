#pragma once
// Copyright (c) 2024 Syntax Errors.
#include "./Object/object.hpp"
#include "./Object/var.hpp"

class List : public Object {
 private:
  std::vector<var> elements;

  size_t normalizeIndex(int index) const {
    if (index < 0) {
      index += (elements.size());
    }
    if (index < 0 || static_cast<size_t>(index) >= elements.size()) {
      throw std::out_of_range("Index out of bounds");
    }
    return static_cast<size_t>(index);
  }

 public:
  List() = default;
  template <typename... Args>
  List(Args&&... args) : elements{var(std::forward<Args>(args))...} {}
  List(std::initializer_list<var> initList) : elements(initList) {}

  // ------------------ Overrides ------------------
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
      return; // Or throw an exception
    }
    elements.insert(elements.begin() + pos, element);
  }

  List slice(int start = 0, int end = -1, int step = 1) const {
    if (step == 0) {
      throw std::invalid_argument("Step cannot be zero");
    }
    size_t normalizedStart = normalizeIndex(start);
    size_t normalizedEnd = normalizeIndex(end);
    List result;
    if (step > 0) {
      for (size_t i = normalizedStart; i < normalizedEnd; i += step) {
        result.addElement(elements[i]);
      }
    } else {
      for (size_t i = normalizedStart; i > normalizedEnd; i += step) {
        result.addElement(elements[i]);
      }
    }
    return result;
  }
  
  size_t size() const {
    return elements.size();
  }

  // ------------------ Operator Overloading ------------------
  
  // Overload the [] operator to access elements by index
  var operator[](int index) const {
    return elements[normalizeIndex(index)];
  }

  // Overload the + operator to concatenate two lists
  List operator+(const List& other) const {
    List result = *this; // Start with a copy of the current list
    result.elements.insert(result.elements.end(), other.elements.begin(), other.elements.end());
    return result;
  }

  // ------------------ Iterator ------------------
    std::vector<var>::iterator begin() {
      return elements.begin();
    }

    std::vector<var>::iterator end() {
      return elements.end();
    }
};