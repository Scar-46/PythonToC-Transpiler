#include "Pair.hpp"

// ------------------ Constructors and destructor ------------------
Pair::Pair() : value() {}
Pair::Pair(var first, var second) : value(std::make_pair(first, second)) {}
Pair::Pair(const Pair& other) : value(other.value) {}
Pair::Pair(Pair&& other) noexcept : value(std::move(other.value)) {}
Pair::~Pair() noexcept = default;

// ------------------ Native overrides ------------------
Pair::operator ObjectPtr() {
  return std::make_shared<Pair>(*this);
}


ObjectPtr Pair::clone() const {
  return std::make_shared<Pair>(*this);
}

void Pair::print(std::ostream& os) const {
  os << "(";
  value.first->print(os);
  os << ", ";
  value.second->print(os);
  os << ")";
}

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

// Non-member swap for ADL
inline void swap(Pair& lhs, Pair& rhs) noexcept {
  lhs.swap(rhs);
}