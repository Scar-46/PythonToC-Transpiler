// Copyright (c) 2024 Syntax Errors.
#include <memory>

#include "Tuple.hpp"
#include "../Object/object.hpp"

void Tuple::init() {
  _methods.erase("pop");
  _methods.erase("clear");
  _methods.erase("remove");
  _methods["index"] = std::bind(&Tuple::index, this, std::placeholders::_1);
}

// Default constructor
Tuple::Tuple() {
  init();
}

// Copy-constructor
Tuple::Tuple(const Tuple& other) : Collection<Tuple, std::vector>(other) {
  init();
}

Tuple::Tuple(const std::vector<var>& elements) : Collection<Tuple, std::vector>(elements) {
  init();
}

// Brace-list constructor
Tuple::Tuple(std::initializer_list<var> initList) : elements(initList) {
  init();
}

// ------------------ Native overrides ------------------
// Print contents
void Tuple::print(std::ostream& os) const {
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
ObjectPtr Tuple::clone() const {
  return std::make_shared<Tuple>(*this);
}

// ------------------ Native operators ------------------
Tuple::operator ObjectPtr() {
  return std::make_shared<Tuple>(*this);
};

bool Tuple::equals(const Object& other) const {
  auto otherTuple = dynamic_cast<const Tuple*>(&other);
  if (!otherTuple) { return false; }

  return _elements == otherTuple->_elements;
}

bool Tuple::less(const Object& other) const {
  auto otherTuple = dynamic_cast<const Tuple*>(&other);
  if (!otherTuple) { return false; }

  return this->_elements < otherTuple->_elements;
}

bool Tuple::greater(const Object& other) const {
  auto otherTuple = dynamic_cast<const Tuple*>(&other);
  if (!otherTuple) { return false; }

  return this->_elements > otherTuple->_elements;
}

// Return a tuple with elements from both tuples (self, then other's)
ObjectPtr Tuple::add(const Object& other) const {
  auto otherTuple = dynamic_cast<const Tuple*>(&other);

  if (!otherTuple) {
    std::cerr << "Invalid argument type, expected Tuple.\n";
    return nullptr;
  }

  std::vector<var> result = this->elements;
  result.insert(
    result.end(), otherTuple->elements.begin(), otherTuple->elements.end());

  return std::make_shared<Tuple>(result);
}


// Access a given element on the collection by index
ObjectPtr Tuple::subscript(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);

  if (!otherObj) {
    std::cerr << "Invalid index type, expected Integer.\n";
    return nullptr;
  }

  std::size_t index = normalizeIndex(otherObj->getValue());

  if (static_cast<size_t>(index) >= _elements.size()) {
    std::cerr << "Invalid index, out of bounds.\n";
    return nullptr;
  }

  return _elements[index].getValue();
}

// ------------------ Management methods ------------------
// Return index of first ocurrence of element
Object::Method::result_type Tuple::index(const std::vector<ObjectPtr>& params) {
  if (params.size() != 1) {
    throw std::runtime_error("index: Invalid number of arguments");
  }

  var query = params[0];
  for (size_t i = 0; i < elements.size(); ++i) {
    if (elements[i] == query) {
      return std::make_shared<Integer>(i);
    }
  }

  std::cerr << "index: Value missing from list" << std::endl;
  return nullptr;
}
