// Copyright (c) 2024 Syntax Errors.
#include "List.hpp"

void List::init() {
  _methods["append"] = std::bind(&List::append, this, std::placeholders::_1);
  _methods["insert"] = std::bind(&List::insert, this, std::placeholders::_1);
  _methods["index"] = std::bind(&List::index, this, std::placeholders::_1);
  _methods["slice"] = std::bind(&List::slice, this, std::placeholders::_1);
}

// Default constructor
List::List() {
  init();
}

// Copy constructor
List::List(const List& other) : Collection<List, std::vector>(other) {
  init();
}

List::List(const std::vector<var>& elements)
  : Collection<List, std::vector>(elements) {
  init();
}

// Brace-list constructor
List::List(std::initializer_list<var> initList)
  : Collection<List, std::vector>(initList) {
  init();
}


// ------------------ Native s -----------------

// Print list contents
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

// Clone self
ObjectPtr List::clone() const {
  return std::make_shared<List>(*this);
}

// ------------------ Native operators ------------------

List::operator ObjectPtr() {
  return std::make_shared<List>(*this);
};

bool List::equals(const Object& other) const {
  auto otherList = dynamic_cast<const List*>(&other);
  if (!otherList) { return false; }

  return _elements == otherList->_elements;
}

bool List::less(const Object& other) const  {
  auto otherList = dynamic_cast<const List*>(&other);
  if (!otherList) { return false; }

  return this->_elements < otherList->_elements;
}

bool List::greater(const Object& other) const  {
  auto otherList = dynamic_cast<const List*>(&other);
  if (!otherList) { return false; }

  return this->_elements > otherList->_elements;
}

// Return a list with elements from both list (self, then other's)
ObjectPtr List::add(const Object& other) const  {
  auto otherList = dynamic_cast<const List*>(&other);
    if (!otherList) {
    std::cerr << "Invalid argument type, expected List.\n";
    return nullptr;
  }

  std::vector<var> result = this->_elements;
  result.insert(
    result.end(), otherList->_elements.begin(), otherList->_elements.end()
  );    // NOLINT

  return std::make_shared<List>(result);
}

// Access a given element on the collection by index
ObjectPtr List::subscript(const Object& other) const  {
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

// ------------------ Management Object::methods ------------------

// Add element to end of list
Object::Method::result_type List::append(const std::vector<ObjectPtr>& params) {
  if (params.size() != 1) {
    throw std::runtime_error("append: Invalid number of arguments");
  }

  ObjectPtr element = params[0];
  if (element) _elements.push_back(element->clone());

  return nullptr;
}

// Insert element on given index
Object::Method::result_type List::insert(const std::vector<ObjectPtr>& params) {
  if (params.size() != 2) {
    throw std::runtime_error("insert: Invalid number of arguments");
  }

  if (! params[0].get()) { 
    std::cerr << "insert: Invalid null index";
    return nullptr;
  }

  const Integer* pos = dynamic_cast<const Integer*>(params[0].get());

  if (!pos ) {
    std::cerr << "insert: Non-integer index";
    return nullptr;
  }

  std::size_t index = normalizeIndex(pos->getValue());
  var element = params[1];

  _elements.insert(_elements.begin() + index, element);
  return nullptr;
}

// Return index of first ocurrence of element
Object::Method::result_type List::index(const std::vector<ObjectPtr>& params) {
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
Object::Object::Method::result_type List::slice(const std::vector<ObjectPtr>& params) {
return generalizedSlice(
  this->_elements,
  params,
  [](std::vector<var>& result, const var& element) { result.push_back(element); },
  [](const std::vector<var>& resultContainer) {
  return std::make_shared<List>(resultContainer);
  }
);    // NOLINT
}
