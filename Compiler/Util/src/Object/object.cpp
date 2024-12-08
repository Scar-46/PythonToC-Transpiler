// Copyright (c) 2024 Syntax Errors.
#include "./object.hpp"
#include "object.hpp"


// Conversion to Object shared pointer 
Object::operator ObjectPtr() {
    throw std::runtime_error("ObjectPtr conversion is not defined by this object");
}

// Conversion operator to bool (can be customized based on logic)
Object::operator bool() const {
    throw std::runtime_error("Boolean conversion not supported for this type");
}

// Conversion to hash (for associative containers)
std::size_t Object::hash() const {
    throw std::runtime_error("Comparison not supported for this type");
}

// Comparison operators
bool Object::equals(unused const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
}

bool Object::less(unused const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
}

bool Object::greater(unused const Object& other) const {
    throw std::runtime_error("Comparison not supported for this type");
}

// Arithmetic operations
ObjectPtr Object::add(unused const Object& other) const {
    throw std::runtime_error("Addition not supported for this type");
}

ObjectPtr Object::subtract(unused const Object& other) const {
    throw std::runtime_error("Subtraction not supported for this type");
}

ObjectPtr Object::multiply(unused const Object& other) const {
    throw std::runtime_error("Multiplication not supported for this type");
}

ObjectPtr Object::divide(unused const Object& other) const {
    throw std::runtime_error("Division not supported for this type");
}

ObjectPtr Object::subscript(unused const Object& other) const {
    throw std::runtime_error("Subscript not supported for this type");
}

// Shift operations
ObjectPtr Object::shiftLeft(unused const Object& other) const {
    throw std::runtime_error("Shift left not supported for this type");
}

ObjectPtr Object::shiftRight(unused const Object& other) const {
    throw std::runtime_error("Shift right not supported for this type");
}

// Check type equivalence
bool Object::isSameType(const Object& other) const {
return typeid(*this) == typeid(other);
}

// ------------------ Per-instance methods ------------------

// Call method supported by object instance
Object::Method::result_type Object::Call(const std::string& name, std::initializer_list<ObjectPtr> params) {
    auto matchedMethod = _methods.find(name);

    if (matchedMethod == _methods.end()) {
        throw std::runtime_error("Object has no method " + name);
    }

    return matchedMethod->second(params);
}

// ------------------ Iterator ------------------

Object::ObjectIt Object::getIterator() const {
    throw std::runtime_error("This object does not support iteration");
}
