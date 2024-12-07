// Copyright (c) 2024 Syntax Errors.
#include "./Set.hpp"  // NOLINT

void Set::init() {
  _methods["add"] = std::bind(&Set::add, this, std::placeholders::_1);
  _methods["union"] = std::bind(&Set::unionW, this, std::placeholders::_1);
  _methods["intersection"] = std::bind(&Set::intersectionW, this, std::placeholders::_1);
  _methods["difference"] = std::bind(&Set::differenceW, this, std::placeholders::_1);
}

// Default constructor
Set::Set() {
  init();
}

// Copy-constructor
Set::Set(const Set& other)
  : Collection<Set, std::unordered_set>(other) {
  init();
}
Set::Set(const std::unordered_set<var>& elements)
  : Collection<Set, std::unordered_set>(elements) {
  init();
};

// Brace-list constructor
Set(std::initializer_list<var> initList)
  : Collection<Set, std::unordered_set>(initList) {
  init();
}

// ------------------ Native overrides ------------------
// Print set contents
void Set::print(std::ostream& os) const {
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
ObjectPtr Set::clone() const {
  return std::make_shared<Set>(*this);
}

// ------------------ Native operators ------------------
Set::operator ObjectPtr() {
  return std::make_shared<Set>(*this);
}

bool Set::equals(const Object& other) const {
  auto otherSet = dynamic_cast<const Set*>(&other);
  if (!otherSet) {
    return false;
  }

  return _elements == otherSet->_elements;
}

  // ------------------ Management Methods ------------------
  // Add element to set
  Object::Method::result_type Set::add(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("add: Invalid number of arguments");
    }

    _elements.insert(params[0]);
    return nullptr;
  }

  // Remove specified element from set
  Object::Method::result_type Set::remove(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("remove: Invalid number of arguments");
    }

    _elements.erase(params[0]);
    return nullptr;
  }

  // Return union of self and another set
  Object::Method::result_type Set::unionW(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("union: Invalid number of arguments");
    }

    const Set* set = dynamic_cast<const Set*>(params[0].get());

    if (!set) {
      std::cerr << "union: Parameter must be Set";
      return nullptr;
    }

    std::unordered_set<var> result = this->_elements;
    result.insert(set->_elements.begin(), set->_elements.end());

    return std::make_shared<Set>(result);
  }

  // Return intersection of self and another set
  Object::Method::result_type Set::intersectionW(const std::vector<ObjectPtr>& params) {
    if (params.size() != 1) {
      throw std::runtime_error("intersection: Invalid number of arguments");
    }

    const Set* other = dynamic_cast<const Set*>(params[0].get());

    if (!other) {
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
Object::Method::result_type Set::differenceW(const std::vector<ObjectPtr>& params) {
  if (params.size() != 1) {
    throw std::runtime_error("difference: Invalid number of arguments");
  }

  const Set* other = dynamic_cast<const Set*>(params[0].get());

  if (!other) {
    std::cerr << "difference: Parameter must be Set";
    return nullptr;
  }

  std::unordered_set<var> result;

  for (const var& element : this->_elements) {
    if (!other->_elements.contains(element)) {
      result.insert(element);
    }
  }

  return std::make_shared<Set>(result);
}
