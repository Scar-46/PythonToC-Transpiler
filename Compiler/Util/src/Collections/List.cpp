// Copyright (c) 2024 Syntax Errors.
#include "List.hpp"

// ------------------ Private methods ------------------

void List::init() {
  _methods["append"] = std::bind(&List::append, this, std::placeholders::_1);
  _methods["insert"] = std::bind(&List::insert, this, std::placeholders::_1);
  _methods["index"] = std::bind(&List::index, this, std::placeholders::_1);
  _methods["slice"] = std::bind(&List::slice, this, std::placeholders::_1);
  _methods["__str__"] = std::bind(&List::asString, this, std::placeholders::_1);
}

// ------------------ Constructors and destructor ------------------

List::List() { init(); }

List::List(const List& other) : Collection<List, std::vector>(other) { init(); }

List::List(const std::vector<var>& elements) : Collection<List, std::vector>(elements) { init(); }

List::~List() = default;

// ------------------ Native overrides ------------------

void List::print(std::ostream& os) const {
  os << "[";
  for (size_t i = 0; i < _elements.size(); ++i) {
    os << _elements[i];
    if (i < _elements.size() - 1) {
      os << ", ";
    }
  }
  os << "]";
}

ObjectPtr List::clone() const {
  return std::make_shared<List>(*this);
}

List::operator ObjectPtr() {
  return std::make_shared<List>(*this);
}

bool List::equals(const Object& other) const {
  auto otherList = dynamic_cast<const List*>(&other);
  return otherList && _elements == otherList->_elements;
}

bool List::less(const Object& other) const {
  auto otherList = dynamic_cast<const List*>(&other);
  return otherList && _elements < otherList->_elements;
}

bool List::greater(const Object& other) const {
  auto otherList = dynamic_cast<const List*>(&other);
  return otherList && _elements > otherList->_elements;
}

ObjectPtr List::add(const Object& other) const {
  auto otherList = dynamic_cast<const List*>(&other);
  if (!otherList) {
    std::cerr << "Invalid argument type, expected List.\n";
    return nullptr;
  }

  std::vector<var> result = _elements;
  result.insert(result.end(), otherList->_elements.begin(), otherList->_elements.end());
  return std::make_shared<List>(result);
}

ObjectPtr List::subscript(const Object& other) const {
  auto otherObj = dynamic_cast<const Integer*>(&other);
  if (!otherObj) {
    std::cerr << "Invalid index type, expected Integer.\n";
    return nullptr;
  }

  std::size_t index = normalizeIndex(otherObj->getValue());
  if (index >= _elements.size()) {
    std::cerr << "Invalid index, out of bounds.\n";
    return nullptr;
  }

  return _elements[index].getValue();
}

// ------------------ Management methods ------------------

ObjectPtr List::append(const std::vector<ObjectPtr>& params) {
  if (params.size() != 1) {
    throw std::runtime_error("append: Invalid number of arguments");
  }

  ObjectPtr element = params[0];
  if (element) _elements.push_back(element->clone());
  return nullptr;
}

ObjectPtr List::insert(const std::vector<ObjectPtr>& params) {
  if (params.size() != 2) {
    throw std::runtime_error("insert: Invalid number of arguments");
  }

  const Integer* pos = dynamic_cast<const Integer*>(params[0].get());
  if (!pos) {
    std::cerr << "insert: Non-integer index\n";
    return nullptr;
  }

  std::size_t index = normalizeIndex(pos->getValue());
  _elements.insert(_elements.begin() + index, params[1]);
  return nullptr;
}

ObjectPtr List::index(const std::vector<ObjectPtr>& params) {
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

ObjectPtr List::slice(const std::vector<ObjectPtr>& params) {
  return generalizedSlice(
    _elements,
    params,
    [](std::vector<var>& result, const var& element) { result.push_back(element); },
    [](const std::vector<var>& resultContainer) {
      return std::make_shared<List>(resultContainer);
    });
}

// Get string representation of list
Object::Method::result_type List::asString(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("__str__: Invalid number of arguments");
  }

  std::string result;
  result.append("[");
  
  for (auto it = _elements.begin(); it != _elements.end(); ++it) {
    if (
      auto stringPtr = std::dynamic_pointer_cast<String>(
        (*it)->Call("__str__", {})
      )
    ) {
      result.append(stringPtr->getValue());
      
      if (std::next(it) != _elements.end()) {
        result.append(", ");
      }
    }
  }

  result.append("]");

  return std::make_shared<String>(result);
}
