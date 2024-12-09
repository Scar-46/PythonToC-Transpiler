// Copyright (c) 2024 Syntax Errors.

#include "./var.hpp"
#include "../Numeric/Double.hpp"
#include "../Numeric/Integer.hpp"
#include "../Primitive/Boolean.hpp"
#include "../Primitive/String.hpp"

// ------------------ var ------------------
var::var() : value(nullptr) {}

// Specialized constructors for base types
var::var(int32_t value) : value(std::make_shared<Integer>(value)) {  }
var::var(double value) : value(std::make_shared<Double>(value)) {  }
var::var(const std::string& value) : value(std::make_shared<String>(value)) {  }
var::var(const char* value) : value(std::make_shared<String>(std::string(value))) {  }
var::var(bool value) : value(std::make_shared<Boolean>(value)) {  }

// Copy constructor and assignment
var::var(const var& other) : value(other.value ? other.value->clone() : nullptr) { }

var& var::operator=(const var& other) {
    if (this != &other) {
        value = other.value ? other.value->clone() : nullptr;
    }
    return *this;
}

// Move constructor and assignment
var::var(var&& other) noexcept : value(std::move(other.value)) {  }

// Copy-assignment from ObjPtr
var::var(const ObjectPtr& obj) : value(obj) {  }

var& var::operator=(const ObjectPtr& other) noexcept {
    value = other;
    return *this;
}

// Access and basic conversion
var::operator bool() const {
    if (!value) { return false; }
    return static_cast<bool>(*value);
}

var::operator const Object&() const {
    if (!value) {
        throw std::runtime_error("Cannot convert null var to Object&");
    }
    return *value;
}

var::operator Object&() {
    if (!value) {
        throw std::runtime_error("Cannot convert null var to Object&");
    }
    return *value;
}

var::operator ObjectPtr&() {
    return value;
}

var::operator const ObjectPtr&() {
    return value;
}

ObjectPtr var::operator->() const {
    return value;
}

ObjectPtr var::getValue() const {
    return this->value;
}

// Comparison operators
bool var::operator==(const var& other) const {
    if (!value || !other.value) {
        if (value == other.value) {
            return true;
        }
        return false;
    }
    return value->equals(*other.value);
}

bool var::operator!=(const var& other) const {
    if (!value || !other.value) {
        if (value == other.value) {
            return false;
        }
        return true;
    }
    return !value->equals(*other.value);
}

std::strong_ordering var::operator<=>(const var& other) const {
    if (*this == other) return std::strong_ordering::equal;
    if (value->less(*other.value)) return std::strong_ordering::less;
    if (value->greater(*other.value)) return std::strong_ordering::greater;

    throw std::runtime_error("Failed three-way comparison");
}

// Hashing for associative containers
std::size_t var::hash() const {
    return value->hash();
}

// Arithmetic operators
var var::operator+(const var& other) const {
    if (!value || !other.value) {
        throw std::runtime_error("Addition not supported for null values");
    }

    return var(value->add(*other.value));
}

var var::operator-(const var& other) const {
    if (!value || !other.value) {
        throw std::runtime_error("Substraction not supported for null values");
    }

    return var(value->subtract(*other.value));
}

var var::operator*(const var& other) const {
    if (!value || !other.value) {
        throw std::runtime_error("Multiplication not supported for null values");
    }

    return var(value->multiply(*other.value));
}

var var::operator/(const var& other) const {
    if (!value || !other.value) {
        throw std::runtime_error("Division not supported for null values");
    }

    return var(value->divide(*other.value));
}

var var::operator[](const var& other) const {
    if (!value || !other.value) {
        throw std::runtime_error("Subscript not supported for null values");
    }

    return var(value->subscript(*other.value));
}

// Print for output
std::ostream& operator<<(std::ostream& os, const var& variable) {
    if (variable.value) {
        variable.value->print(os);
    } else {
        os << "None";
    }
    return os;
}

// Iterators

Iterator var::getIterator() const {
if (!value) {
    throw std::runtime_error("Cannot retrieve iterator from null var");
}
return Iterator(value->getIterator());
}

Iterator var::cbegin() const {
    if (!value) {
        throw std::runtime_error("Cannot iterate over null var");
    }

    return Iterator(value->getIterator());
}

Iterator var::cend() const {
    return Iterator();
}

Iterator var::begin() {
    if (!value) {
        throw std::runtime_error("Cannot iterate over null var");
    }
    return Iterator(value->getIterator());
}

Iterator var::end() {
    return Iterator();
}

// Specific methods per instance
ObjectPtr var::Call(const std::string& name, std::initializer_list<ObjectPtr> params) {
    if (!value) {
        throw std::runtime_error("Cannot call method on null var");
    }

    return value->Call(name, params);
}

// ------------------ Iterator ------------------

Iterator::Iterator(Object::ObjectIt iterator): objectIterator(std::move(iterator)), isEnd(false) {this->init();}

Iterator::Iterator() : objectIterator(nullptr), isEnd(true) {this->init();}

Iterator::Iterator(const Iterator& other)
    : objectIterator(other.objectIterator), isEnd(other.isEnd) { this->init(); }

Iterator& Iterator::operator++() {
    if (!objectIterator || isEnd) {
      throw std::runtime_error("Incrementing an invalid iterator");
    }
    if (!objectIterator->hasNext()) {
      isEnd = true;
    }
    return *this;
  }

bool Iterator::operator!=(const Iterator& other) const {
    return isEnd != other.isEnd;  // Simple end-check comparison
}

var Iterator::operator*() const {
    if (!objectIterator || isEnd || !objectIterator->hasNext()) {
        throw std::runtime_error("Dereferencing an invalid iterator");
    }

    return var(objectIterator->next());
}

void Iterator::print(std::ostream& os) const { os << ""; }

ObjectPtr Iterator::clone() const {
    auto clonedIterator = std::make_shared<Iterator>();

    if (objectIterator) {
        clonedIterator->objectIterator = objectIterator->clone(); // Assuming objectIterator has a clone method
    }
    clonedIterator->isEnd = this->isEnd;

    return clonedIterator;
}

var Iterator::operator*() {
    if (!objectIterator || isEnd || !objectIterator->hasNext()) {
        throw std::runtime_error("Dereferencing an invalid iterator");
    }

    return var(objectIterator->next());
}

void Iterator::init() {
    _methods["__next__"] = std::bind(&Iterator::next, this, std::placeholders::_1);
    _methods["__bool__"] = std::bind(&Iterator::asBoolean, this, std::placeholders::_1);
}

Object::Method::result_type Iterator::next(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
        throw std::runtime_error("__next__: Invalid number of arguments");
    }

    if (isEnd || !objectIterator) {
        throw std::runtime_error("No more elements to iterate over");
    }

    var current = **this;

    ++(*this);
    return current.getValue();
}

Object::Method::result_type Iterator::asBoolean(const std::vector<ObjectPtr>& params) {
    if (params.size() != 0) {
        throw std::runtime_error("__bool__: Invalid number of arguments");
    }

    return std::make_shared<Boolean>(!isEnd && !objectIterator);
}