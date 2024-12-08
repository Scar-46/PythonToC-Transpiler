#include "Pair.hpp"
#include "../Primitive/String.hpp"
#include "../Primitive/Boolean.hpp"
#include "../Numeric/Integer.hpp"

Pair::Pair() { init(); }

Pair::operator ObjectPtr() {
  init();
  return std::make_shared<Pair>(*this);
}

// Parameterized constructor
Pair::Pair(var first, var second)
  : value(std::make_pair(first, second)) { init(); }

// Copy constructor
Pair::Pair(const Pair& other)
  : value(other.value) { init(); }

// Move constructor
Pair::Pair(Pair&& other) noexcept
  : value(std::move(other.value)) { init(); }

// ------------------ Native operators ------------------
Pair& Pair::operator=(const Pair& other) {
  if (this != &other) {
    value = other.value;
  }
  return *this;
}

Pair& Pair::operator=(Pair&& other) noexcept {
  if (this != &other) {
    value = std::move(other.value);
  }
  return *this;
}

inline bool Pair::operator==(const Pair& other) const {
  return value == other.value;
}

bool Pair::operator!=(const Pair& other) const {
  return !(*this == other);
}

bool Pair::operator<(const Pair& other) const {
  return value < other.value;
}

bool Pair::operator<=(const Pair& other) const {
  return value <= other.value;
}

bool Pair::operator>(const Pair& other) const {
  return value > other.value;
}

bool Pair::operator>=(const Pair& other) const {
  return value >= other.value;
}

bool Pair::equals(const Object& other) const {
  auto otherObj = dynamic_cast<const Pair*>(&other);
  if (otherObj) {
    return value == otherObj->value;
  }
  throw std::runtime_error("Pair does not support comparison with given type");
}

bool Pair::less(const Object& other) const {
  auto otherObj = dynamic_cast<const Pair*>(&other);
  if (otherObj) {
    return value < otherObj->value;
  }
  throw std::runtime_error("Pair does not support comparison with given type");
}

bool Pair::greater(const Object& other) const {
  auto otherObj = dynamic_cast<const Pair*>(&other);
  if (otherObj) {
    return value > otherObj->value;
  }
  throw std::runtime_error("Pair does not support comparison with given type");
}

// ------------------ Accessors ------------------
var Pair::getFirst() const {
  return value.first;
}

void Pair::setFirst(const var& first) {
  value.first = first;
}

var Pair::getSecond() const {
  return value.second;
}

void Pair::setSecond(const var& second) {
  value.second = second;
}

void Pair::swap(Pair& other) noexcept {
  std::swap(value, other.value);
}

// Print function
void Pair::print(std::ostream& os) const {
  os << "(";
  value.first->print(os);
  os << ", ";
  value.second->print(os);
  os << ")";
}

ObjectPtr Pair::clone() const {
  return std::make_shared<Pair>(*this);
};

void Pair::init() {
  _methods["__len__"] = std::bind(&Pair::len, this, std::placeholders::_1);
  _methods["__bool__"] = std::bind(&Pair::asBoolean, this, std::placeholders::_1);
  _methods["__str__"] = std::bind(&Pair::asString, this, std::placeholders::_1);
}

// Return two
Object::Method::result_type Pair::len(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("__str__: Invalid number of arguments");
  }

  return std::make_shared<Integer>(2);
}

// Get string representation of pair
Object::Method::result_type Pair::asString(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("__str__: Invalid number of arguments");
  }

  std::string result;
  
  result.append("(");
  
  if (
    auto stringPtr = std::dynamic_pointer_cast<String>(
      this->value.first.Call("__str__", {})
    )
  ) {
    result.append(stringPtr->getValue());
  };

  result.append(", ");

  if (
    auto stringPtr = std::dynamic_pointer_cast<String>(
      this->value.second.Call("__str__", {})
    )
  ) {
    result.append(stringPtr->getValue());
  };

  result.append(")");

  return std::make_shared<String>(result);
}

// Return true
Object::Method::result_type Pair::asBoolean(const std::vector<ObjectPtr>& params) {
  if (params.size() != 0) {
    throw std::runtime_error("__bool__: Invalid number of arguments");
  }

  return std::make_shared<Boolean>(true);
}

// Non-member swap for ADL
inline void swap(Pair& lhs, Pair& rhs) noexcept {
  lhs.swap(rhs);
}