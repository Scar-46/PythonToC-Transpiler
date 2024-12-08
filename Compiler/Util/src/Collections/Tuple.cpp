// Copyright (c) 2024 Syntax Errors.
#include <memory>
#include <string>

#include "Tuple.hpp"
#include "../Object/object.hpp"

void Tuple::init() {
  _methods.erase("pop");
  _methods.erase("clear");
  _methods.erase("remove");
  _methods["index"] = std::bind(&Tuple::index, this, std::placeholders::_1);
  _methods["__str__"] = std::bind(&Tuple::asString, this, std::placeholders::_1);
}

// Default constructor
Tuple::Tuple() {
  init();
}

// Copy-constructor
Tuple::Tuple(const Tuple& other) : Collection<Tuple, std::vector>(other) {
  init();
}

Tuple::Tuple(const std::vector<var>& _elements) : Collection<Tuple, std::vector>(_elements) {
  init();
}

// ------------------ Native overrides ------------------
// Print contents
void Tuple::print(std::ostream& os) const {
  os << "(";
  for (size_t i = 0; i < _elements.size(); ++i) {
    os << _elements[i];
    if (i < _elements.size() - 1) {
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

// Return a tuple with _elements from both tuples (self, then other's)
ObjectPtr Tuple::add(const Object& other) const {
  auto otherTuple = dynamic_cast<const Tuple*>(&other);

  if (!otherTuple) {
    std::cerr << "Invalid argument type, expected Tuple.\n";
    return nullptr;
  }

  std::vector<var> result = this->_elements;
  result.insert(
    result.end(), otherTuple->_elements.begin(), otherTuple->_elements.end());

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
  for (size_t i = 0; i < _elements.size(); ++i) {
    if (_elements[i] == query) {
      return std::make_shared<Integer>(i);
    }
  }

  std::cerr << "index: Value missing from list" << std::endl;
  return nullptr;
}

Object::Method::result_type Tuple::asString(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("__str__: Invalid number of arguments");
  }

  std::string result;
  result.append("(");

  for (size_t i = 0; i < _elements.size(); ++i) {
    if (
      auto stringPtr = std::dynamic_pointer_cast<String>(
        _elements[i]->Call("__str__", {})
      )
    ) {
      result.append(stringPtr->getValue());

      if (i + 1 != _elements.size()) {
        result.append(", ");
      }
    }
  }

  result.append(")");

  return std::make_shared<String>(result);
}
